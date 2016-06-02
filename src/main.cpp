#include <gl.h>

using namespace std;

int WINDOW_WIDTH, WINDOW_HEIGHT;
unsigned int vbo, ibo_buffer, program, menu;
unsigned int f_color_loc, world_loc, coord_loc, angle_loc, camera_loc, scale_loc;
polygon* Field;
Matrix3f World;
Matrix3f Camera;
int amount_of_polygons;
int ibo_size;
ship* ships;
int amount_of_ships;
int curr_ship;
field field1, field2;
int mouse_x = WINDOW_WIDTH / 2, mouse_y = WINDOW_HEIGHT / 2;
float field_color[] = {0.9, 0.9, 0.9, 0.7};
float ship_color[] = {1, 0.5, 1, 1}, current_ship_color[] = {0.7, 0, 0.4, 1};
float bomb_color[] = {0.7, 0, 0, 1}, aqua_color[] = {0, 0.4, 0.8, 1};
bool window_should_close = false, play_audio = true, turning = false;
int cnt;
background bg;
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
        check(field1, ships);
    } else if (key == ' ') {
        turning = true;
        curr_ship = -1;
    }
}

void MouseEvent(int button, int state, int x, int y) {
    y = 768 - y;
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
    if (x < 0 or x > WINDOW_WIDTH) return;
    if (y < 0 or y > WINDOW_HEIGHT) return;
    if (x < WINDOW_WIDTH / 16) {
        Camera.m[2] += CONST_SPEED_CAMERA * (WINDOW_WIDTH / 4 - x) / 1000;
    }
    if (x > WINDOW_WIDTH * 15 / 16) {
        Camera.m[2] -= CONST_SPEED_CAMERA * (x - 3 * WINDOW_WIDTH / 4)  / 1000;
    }
    if (y < WINDOW_HEIGHT / 16) {
        Camera.m[5] -= CONST_SPEED_CAMERA * (WINDOW_HEIGHT / 4 - y) / 500;
    }
    if (y > WINDOW_HEIGHT * 15 / 16) {
        Camera.m[5] += CONST_SPEED_CAMERA * (y - 3 * WINDOW_HEIGHT / 4)  / 500;
    }
    Camera.m[2] = max(-500.0f, Camera.m[2]);
    Camera.m[5] = max(-500.0f, Camera.m[5]);
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
        draw_ship(i, ship_color);
    }
    draw_bombs(field1);
    draw_bombs(field2);
    if (curr_ship != -1) {
        draw_ship(curr_ship, current_ship_color);
    }
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
    init_matrixes();
    init_ship_object();
    init_fields();
    init_colors();
    create_field_vbo();
    cout << "created" << endl;
    init_resourses();
    while (!window_should_close) {
        glutMainLoop();
    }
    SDL_CloseAudio();
    SDL_FreeWAV(wav_buffer);
    return 0;
}

