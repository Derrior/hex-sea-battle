#include <GL/glew.h>
#include <music_player.h>
#include <GL/freeglut.h>
#include <background.h>
#include <math_3d.h>
#include <polygon.h>
#include <drawing.h>
#include <shader.h>
#include <iostream>
#include <engine.h>
#include <field.h>
#include <geom.h>
#include <cstdio>
#include <file.h>
#include <ship.h>
#include <gl.h>

int WINDOW_WIDTH;
int WINDOW_HEIGHT;
#define CONST_SPEED_CAMERA 20


using namespace std;

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
float ship_color[] = {1, 0.5, 1, 1}, current_ship_color[] = {0.7, 0, 0.4, 1};
float bomb_color[] = {1, 0, 0, 1}, aqua_color[] = {0, 0.5, 1, 1};
bool window_should_close = false, play_audio = true, turning = false;
int cnt;
background bg;

void init_matrixes() {
    for (int i = 0; i < 6; i++) {
        matrixes[i][0] = matrixes[i][3] = cosf(i * (M_PI / 3));
        matrixes[i][1] = -sinf(i * (M_PI / 3));
        matrixes[i][2] = -matrixes[i][1];
    }
}

void set_ship(int& x, int& y, field& F) {
    for (int i = 0; i < amount_of_polygons; i++) {
        if (in_polygon(point(x - F.move.m[2], y - F.move.m[5]), Field[i])) {
            x = Field[i].centre.x + F.move.m[2];
            y = Field[i].centre.y + F.move.m[5];
        }
    }
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
        check(field1, ships);
    } else if (key == ' ') {
        turning = true;
    }
}
/*
0 1 2
3 4 5
6 7 8
*/

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

void PressSpecial(int key, int, int) {
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

void PressMenu(int v) {
    cout << v << endl;
}

void draw_hex(int idx)
{
    glBegin(GL_POLYGON);
    glColor4d(122.0, 122.0, 122.0, 1);
    for (int i = 0; i < (int)Field[idx].points.size(); i++)
    {
        glVertex2f(Field[idx].points[i].x, Field[idx].points[i].y);
    }
    glEnd();
}
/*
static void init_menu() {
    menu = glutCreateMenu(PressMenu);
    glutAddMenuEntry("Press", 1);
    glutAddMenuEntry("NOPress", 2);
}
*/


static void RenderSceneCB()
{
    if (play_audio) {
        SDL_Delay(1);
    }
    WINDOW_HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);
    WINDOW_WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    PassiveMotionEvent(mouse_x, mouse_y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5, 0.5, 0.5, 1);
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
 
/*void Pointer(int x, int y)
{
    
}*/
static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
    glutKeyboardFunc(&PressEvent);
    glutSpecialFunc(PressSpecial);
    glutMouseFunc(MouseEvent);
    glutPassiveMotionFunc(PassiveMotionEvent);
}


void init_resourses()
{
    GLuint vs = create_shader("shader/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fs = create_shader("shader/fragment.glsl", GL_FRAGMENT_SHADER);
    program = create_program(vs, fs);
    glUseProgram(program);
    world_loc = glGetUniformLocation(program, "World");
    scale_loc = glGetUniformLocation(program, "scale");
    angle_loc = glGetUniformLocation(program, "rotate");
    camera_loc = glGetUniformLocation(program, "camera");
    f_color_loc = glGetUniformLocation(program, "f_color");
    //init_menu();
}


static void CreateVertexBuffer()
{

    float points[14 * amount_of_polygons];
    vector<int> ibo;
    for (int i = 0; i < amount_of_polygons; i++)
    {
        draw_polygon(Field[i], points, i * 14, ibo);
    }
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    unsigned int ibo_data[ibo.size()];
    for (int i = 0; i < (int)ibo.size(); i++) {
        ibo_data[i] = ibo[i];
    }
    glGenBuffers(1, &ibo_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibo_data), ibo_data, GL_STATIC_DRAW);
    ibo_size = ibo.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    create_ships();
    //glutSetCursor(0, 0);
}

void init_fields() {
    field1 = field(amount_of_polygons);
    field2 = field(amount_of_polygons);
    field1.move.m[2] = field1.move.m[5] = field2.move.m[5] = 40;
    field2.move.m[2] = 860;
    field1.bombs.push_back(19);
}


int main(int argc, char** argv)
{
    sdl_init();
    SDL_AudioSpec wav_spec;
    if (!SDL_LoadWAV("sum.wav", &wav_spec, &wav_buffer,&wav_length)) {
        play_audio = false;;
    };
    if (play_audio) {
        wav_spec.callback = my_audio_callback;
        wav_spec.userdata = NULL;
        audio_pos = wav_buffer;
        audio_len = wav_length;

        SDL_OpenAudio(&wav_spec, NULL);
        SDL_PauseAudio(0);
    }
    Field = gen_field(10, 10, 1);
    amount_of_polygons = 100;
    World.m[2] = -100;
    World.m[5] = -100;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1366, 768);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Tutorial 01");

    WINDOW_HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);
    WINDOW_WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    InitializeGlutCallbacks();
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }
    glClearColor(0.5f, .5f, .5f, .5f);
    init_matrixes();
    init_ship_object();
    init_fields();
    init_colors();
    CreateVertexBuffer();
    cout << "created" << endl;
    init_resourses();
    while (!window_should_close) {
        glutMainLoop();
    }
    SDL_CloseAudio();
    SDL_FreeWAV(wav_buffer);
    return 0;
}

