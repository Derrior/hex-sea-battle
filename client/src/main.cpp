#include <net.h>
#include <SOIL/SOIL.h>
#include <gl.h>
#include <ctime>
#include <iostream>
using namespace std;

int WINDOW_WIDTH, WINDOW_HEIGHT;
unsigned int vbo, ibo_buffer, program, menu;
unsigned int f_color_loc, world_loc, coord_loc, angle_loc, camera_loc, scale_loc, tex_loc, tex_coord_loc, any_texture_loc, shadowing_loc;
polygon* Field;
Matrix3f World;
Matrix3f Camera;
Matrix3f Empty;
float world_scale;
int amount_of_polygons, amount_of_ships, curr_ship;
int ibo_size;
int colorscheme;
ship* ships;
field field1, field2;
GLuint tex, tex_a[128];
int mouse_x = WINDOW_WIDTH / 2, mouse_y = WINDOW_HEIGHT / 2;
float field_color[] = {0.9, 0.9, 0.9, 0.4,
                       0.1, 0.1, 0.1, 0.4,
                       0.3, 0.1, 0.5, 0.7,
                       0.9, 0.5, 0.1, 0.7}, white_color[] = {1, 1, 1, 1}, black_color[] = {0, 0, 0, 0};
float ship_color[] = {1, 0.5, 1, 1,
                      0.8, 0.6, 0, 1,
                      0.2, 0.6, 0, 1,
                      0.2, 0.2, 0.2, 1},
current_ship_color[] = {0.7, 0, 0.4, 1,
                        0.8, 0.4, 0, 1,
                        0.9, 0.8, 0.1, 1,
                        0.2, 0.2, 0.6, 1
                      };

float bomb_color[] = {0.7, 0, 0, 1}, aqua_color[] = {0, 0.4, 0.8, 1};
bool bombs_removed, window_should_close = false, play_audio = true, check_result = true;
int cnt;
long double curr_time;
long long time_last_check;
bool need_next_mode, active_buttons = true;
background bg;
vector<vector<button> > buttons;
SDL_AudioSpec wav_spec;


void next_mode() {
    switch (mode) {
    case INIT_MODE:
        mode = SHIP_MODE;
        break;
    case SHIP_MODE:
        mode = BATTLE_MODE;
        break;
    case BATTLE_MODE:
        mode = INIT_MODE;
        break;
    }
    cout << "Current mode - " << mode << endl;
}

void PressEvent(unsigned char key, int x, int y) {
    y = 768 - y;
    x -= Camera.m[2];
    y -= Camera.m[5];
    if (key == 'w') {
        ships[curr_ship].pos.m[5] += D_X;
    } else if (key == 's') {
        ships[curr_ship].pos.m[5] -= D_X;
    } else if (key == 'a') {
        ships[curr_ship].pos.m[2] -= D_X; 
    } else if (key == 'd') {
        ships[curr_ship].pos.m[2] += D_X;
    } else if (key == 'c') {
        set_ship(x, y, field1);
        ships[curr_ship].pos.m[2] = x;
        ships[curr_ship].pos.m[5] = y;

    } else if (key == 't'){
        Camera.m[2] += 10;
        Camera.m[2] = min(50.0f, Camera.m[2]);
        Camera.m[5] = min(50.0f, Camera.m[5]);
    } else if (key == 'e'){
        Camera.m[2] -= 10;
        Camera.m[2] = min(50.0f, World.m[2]);
    } else if (key == 27) {
        glutDestroyWindow(1);
    } else if (key == 13) {
        time_last_check = time(NULL);
        bombs_removed = false;
        check();

    } else if (key == ' ') {
        curr_ship = -1;
    } else if (key == '-') {
        world_scale *= 0.95;
    } else if (key == '+') {
        world_scale /= 0.95;
    }
}

void MouseEvent(int button, int state, int x, int y) {
    y = 768 - y;
    if (active_buttons) {
        for (int i = 0; i < (int)buttons[mode].size(); i++) {
            if (buttons[mode][i].is_pressed(point(x, y))) {
                buttons[mode][i].call_callback();
                cout << "try to catch" << endl;
                return;
            }
        }
    }
    x -= Camera.m[2];
    y -= Camera.m[5];
    if (mode == SHIP_MODE) {
        if (button == GLUT_RIGHT_BUTTON and state == GLUT_DOWN) {
            if (curr_ship == -1) {
                for (int i = 0; i < amount_of_ships; i++) {
                    if (ships[i].in_ship(point(x, y))) {
                        curr_ship = i;
                    }
                }
            } else {
                set_ship(x, y, field1);
                ships[curr_ship].pos.m[2] = x;
                ships[curr_ship].pos.m[5] = y;
                curr_ship = -1;
            }
        }
        if (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN) {
            if (curr_ship == -1) {
                for (int i = 0; i < amount_of_ships; i++) {
                    if (ships[i].in_ship(point(x, y))) {
                        curr_ship = i;
                    }
                }
            } 
        } else if (button == GLUT_LEFT_BUTTON and state == GLUT_UP and curr_ship != -1) {
            x = ships[curr_ship].pos.m[2];
            y = ships[curr_ship].pos.m[5];
            set_ship(x, y, field1);
            ships[curr_ship].pos.m[2] = x;
            ships[curr_ship].pos.m[5] = y;
            curr_ship = -1;
        }
    } else {
        if (button == GLUT_LEFT_BUTTON and state == GLUT_UP) {
            x -= field1.move.m[2];
            y -= field1.move.m[5];
            if (!field1.used[get_cell_idx(point(x, y))]) {
                turn(x, y, field1, ships);
                field1.used[get_cell_idx(point(x, y))] = true;
            }

        }
    }
    
}

void MotionEvent(int x, int y) {
    if (curr_ship != -1) {
        ships[curr_ship].pos.m[2] += x - mouse_x;
        ships[curr_ship].pos.m[5] += mouse_y - y;
    } else {
        Camera.m[2] += (x - mouse_x);
        Camera.m[5] += (mouse_y - y);
    }
    mouse_x = x;
    mouse_y = y;
}

void PassiveMotionEvent(int x, int y) {
    y = 768 - y;

    if (x < 0 or x > WINDOW_WIDTH) return;
    if (y < 0 or y > WINDOW_HEIGHT) return;
    for (int i = 0; i < (int)buttons[mode].size(); i++) {
	if (distance_m((buttons[mode][i].place), point(x, y)) < 100) {
        y = 768 - y;
        mouse_x = x;
        mouse_y = y;
		return;
	}

    }
    if (x < WINDOW_WIDTH / 16) {
        Camera.m[2] += CONST_SPEED_CAMERA * (WINDOW_WIDTH / 4 - x) / 1000;
    }
    if (x > WINDOW_WIDTH * 15 / 16) {
        Camera.m[2] -= CONST_SPEED_CAMERA * (x - 3 * WINDOW_WIDTH / 4)  / 1000;
    }
    if (y < WINDOW_HEIGHT / 16) {
        Camera.m[5] += CONST_SPEED_CAMERA * (WINDOW_HEIGHT / 4 - y) / 500;
    }
    if (y > WINDOW_HEIGHT * 15 / 16) {
        Camera.m[5] -= CONST_SPEED_CAMERA * (y - 3 * WINDOW_HEIGHT / 4)  / 500;
    }
    Camera.m[2] = max(-500.0f, Camera.m[2]);
    Camera.m[5] = max(-500.0f, Camera.m[5]);
    y = 768 - y;
    mouse_x = x;
    mouse_y = y;
}

void SpecialEvent(int key, int, int) {
    if (key == GLUT_KEY_RIGHT) {
        curr_ship += 1;
    } else if (key == GLUT_KEY_LEFT) {
        curr_ship += amount_of_ships - 1;
    } else if (key == GLUT_KEY_UP) {
        ships[curr_ship].unrotate();
    } else if (key == GLUT_KEY_DOWN) {
        ships[curr_ship].rotate();
    }
    curr_ship %= amount_of_ships;
}



static void update_all() {
    curr_time = (float)clock() / CLOCKS_PER_SEC;
    WINDOW_HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);
    WINDOW_WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    PassiveMotionEvent(mouse_x, mouse_y);

    if (go_allowed) {
        cout << 1 << endl;
        begin_switch_mode = curr_time;
        go_pressed = go_allowed = 0;
        need_next_mode = true;
        active_buttons = false;
    }
    if (need_next_mode and SWITCH_DUR - (curr_time - begin_switch_mode) < EPS) {
        next_mode();
        need_next_mode = false;
        active_buttons = true;
    }

    if (mode == SHIP_MODE) {
        if (!bombs_removed and (long long)curr_time - time_last_check > BOMB_CONST) {
            field1.bombs.clear();
            field2.bombs.clear();
            bombs_removed = true;
        }
    }
    update_net();
}
 

static void RenderSceneCB()
{ 
    update_all();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5, 0.5, 0.5, 0);
    long double shadowing = min(1.0, fabs(((float)SWITCH_DUR - (curr_time - begin_switch_mode)) / (float)SWITCH_DUR));
    glUniform1f(shadowing_loc, shadowing);
    glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Camera.m[0]);
    glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
    draw_background();
    if (mode != INIT_MODE) {
        draw_field(field1);
        draw_field(field2);
        
        for (int i = 0; i < amount_of_ships; i++) {
            if (i != curr_ship)
            draw_ship(i, ship_color + 4 * colorscheme);
        }
        draw_bombs(field1);
        draw_bombs(field2);
        if (curr_ship != -1) {
            draw_ship(curr_ship, current_ship_color + 4 * colorscheme);
        }
    }
    draw_buttons();
//  Test zone
//end of test zone

    glutSwapBuffers();
}

static void InitializeGlutCallbacks() {
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
    glutKeyboardFunc(PressEvent);
    glutSpecialFunc(SpecialEvent);
    glutMouseFunc(MouseEvent);
    glutMotionFunc(MotionEvent);
    glutPassiveMotionFunc(PassiveMotionEvent);
}


void init_resourses() {
    GLuint vs = create_shader("shader/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fs = create_shader("shader/fragment.glsl", GL_FRAGMENT_SHADER);
    program = create_program(vs, fs);
    glUseProgram(program);
    world_loc = glGetUniformLocation(program, "World");
    scale_loc = glGetUniformLocation(program, "scale");
    angle_loc = glGetUniformLocation(program, "rotate");
    camera_loc = glGetUniformLocation(program, "camera");
    f_color_loc = glGetUniformLocation(program, "f_color");
    tex_loc = glGetUniformLocation(program, "tex");
    any_texture_loc = glGetUniformLocation(program, "using_textures");
    shadowing_loc = glGetUniformLocation(program, "shadowing");
    tex_coord_loc = glGetAttribLocation(program, "tex_coord");
    glDepthMask(false);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
}

#ifdef _WIN32
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	main(0, NULL);

}
#endif


int main(int argc, char** argv)
{
    curr_time = time(NULL);
    sdl_init();
    init_audio();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Tutorial 01");
    glutFullScreen();

    WINDOW_HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);
    WINDOW_WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    InitializeGlutCallbacks();
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }
    glClearColor(0.5f, .5f, .5f, .0f);
    /*
    SDL_Surface* screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 
    apply_surface(10, 10, image, screen);
    */
    init_matrixes();
    init_ship_object();
    init_fields();
    init_colors();
    init_buttons();
    cout << "beforeft" << endl;
    init_ft();
    create_field_vbo();
    cout << "created" << endl;
    init_resourses();
    //glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    int width, height;
    //unsigned char* image = SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGBA);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    //SOIL_free_image_data(image);
    if (init_net("127.0.0.1", PORT)) {
        cout << "connection failed" << endl;
        return 1;
    }
    while (!window_should_close) {
        glutMainLoop();
    }
    free_net();
    SDL_CloseAudio();
    SDL_FreeWAV(wav_buffer);
    return 0;
}

