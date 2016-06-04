#include <gl.h>
#include <ft.h>
#include <ctime>
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>
using namespace std;


int WINDOW_WIDTH, WINDOW_HEIGHT;
unsigned int vbo, ibo_buffer, program, menu;
unsigned int f_color_loc, world_loc, coord_loc, angle_loc, camera_loc, scale_loc, tex_loc, aa, any_texture_loc;
polygon* Field;
Matrix3f World;
Matrix3f Camera;
Matrix3f Empty;
int amount_of_polygons, amount_of_ships, curr_ship;
int ibo_size;
int colorscheme;
ship* ships;
field field1, field2;
GLuint tex, tex_a[128];
int mouse_x = WINDOW_WIDTH / 2, mouse_y = WINDOW_HEIGHT / 2;
float field_color[] = {0.9, 0.9, 0.9, 0.4,
                       0.1, 0.1, 0.1, 0.4,
                       0.3, 0.1, 0.5, 0.4,
                       0.6, 0.4, 0.1, 0.4}, white_color[] = {1, 1, 1, 1}, black_color[] = {0, 0, 0, 0};
float ship_color[] = {1, 0.5, 1, 1}, current_ship_color[] = {0.7, 0, 0.4, 1};
float bomb_color[] = {0.7, 0, 0, 1}, aqua_color[] = {0, 0.4, 0.8, 1};
bool bombs_removed, window_should_close = false, play_audio = true, turning = false;
int cnt;
long double curr_time;
long long time_last_check;
background bg;
vector<button> buttons;
SDL_AudioSpec wav_spec;


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
        check(field1, ships);

    } else if (key == ' ') {
        turning = true;
        curr_ship = -1;
    }
}

void MouseEvent(int button, int state, int x, int y) {
    y = 768 - y;
    for (int i = 0; i < (int)buttons.size(); i++) {
        if (buttons[i].is_pressed(point(x, y))) {
            buttons[i].call_callback();
            cout << "try to catch" << endl;
            return;
        }
    }
    x -= Camera.m[2];
    y -= Camera.m[5];
    if (!turning) {
        if (button == GLUT_LEFT_BUTTON and state == GLUT_UP) {
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

void PassiveMotionEvent(int x, int y) {
    y = 768 - y;

    if (x < 0 or x > WINDOW_WIDTH) return;
    if (y < 0 or y > WINDOW_HEIGHT) return;
    for (int i = 0; i < (int)buttons.size(); i++) {
	if (distance_m((buttons[i].place), point(x, y)) < 100) {
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

static void RenderSceneCB()
{ 
    curr_time = time(NULL);
    WINDOW_HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);
    WINDOW_WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    PassiveMotionEvent(mouse_x, mouse_y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5, 0.5, 0.5, 0);
    glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Camera.m[0]);
    glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
    draw_background();
    draw_field(field1);
    draw_field(field2);
    
    for (int i = 0; i < amount_of_ships; i++) {
        if (i != curr_ship)
        draw_ship(i, ship_color);
    }
    if (!turning) {
        if (!bombs_removed and (long long)curr_time - time_last_check > BOMB_CONST) {
            field1.bombs.clear();
            field2.bombs.clear();
            bombs_removed = true;
        }
    }
    draw_bombs(field1);
    draw_bombs(field2);
    draw_buttons();
    if (curr_ship != -1) {
        draw_ship(curr_ship, current_ship_color);
    }
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
    aa = glGetAttribLocation(program, "tex_coord");
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
    unsigned char* image = SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);


    while (!window_should_close) {
        glutMainLoop();
    }
    SDL_CloseAudio();
    SDL_FreeWAV(wav_buffer);
    return 0;
}

