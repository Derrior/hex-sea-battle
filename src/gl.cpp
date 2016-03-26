#include <GL/glew.h>
#include <GL/freeglut.h>
#include <background.h>
#include <math_3d.h>
#include <polygon.h>
#include <shader.h>
#include <iostream>
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

unsigned int vbo, ibo_buffer, program;
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
float ship_color[] = {1, 0.5, 1, 1}, current_ship_color[] = {0, 1, 0, 1};
float bomb_color[] = {1, 0, 0, 1};
bool window_should_close = false;
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
        field2.bombs.push_back(cnt++);
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
        Camera.m[5] -= CONST_SPEED_CAMERA * (WINDOW_HEIGHT / 4 - y) / 1000;
    }
    if (y > WINDOW_HEIGHT * 15 / 16) {
        Camera.m[5] += CONST_SPEED_CAMERA * (y - 3 * WINDOW_HEIGHT / 4)  / 1000;
    }
    Camera.m[2] = max(0.0f, Camera.m[2]);
    Camera.m[5] = max(0.0f, Camera.m[5]);
    mouse_x = x;
    mouse_y = y;
}

void PressSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
        curr_ship += 1;
    } else if (key == GLUT_KEY_LEFT) {
        curr_ship += amount_of_ships - 1;
    } else if (key == GLUT_KEY_UP) {
        ships[curr_ship].rotate();
    } 
    curr_ship %= amount_of_ships;
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

static void draw_cell(int cell_idx, const float* color, field& F) {
        World.m[2] = F.move.m[2] + Field[cell_idx].centre.x;
        World.m[5] = F.move.m[5] + Field[cell_idx].centre.y;
        glUniformMatrix3fv(world_loc, 1, GL_TRUE, &World.m[0]);
        glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
        glUniform4fv(f_color_loc, 1, color);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
        glDrawElements(GL_TRIANGLES, SHIP_SIZE, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
}

static void draw_cell(int cell_idx) {
        World.m[2] = bg.arr[cell_idx].centre.x;
        World.m[5] = bg.arr[cell_idx].centre.y;
        glUniformMatrix3fv(world_loc, 1, GL_TRUE, &World.m[0]);
        glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Camera.m[0]);
        glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
        glUniform4fv(f_color_loc, 1, &colors[bg.color_idx[cell_idx]].c[0]);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
        glDrawElements(GL_TRIANGLES, SHIP_SIZE, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
    
}

static void draw_background() {
    glUniform1f(scale_loc, BACKGROUND_CELL_RAD);
    for (int i = 0; i < (int)bg.arr.size(); i++) {
        draw_cell(i);
        
    }
    glUniform1f(scale_loc, 1);
}

static void draw_ship(int ship_idx, const float* color) {
        glUniformMatrix3fv(world_loc, 1, GL_TRUE, &ships[ship_idx].pos.m[0]);
        glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[ships[ship_idx].rot][0]);
        glUniform4fv(f_color_loc, 1, color);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
        glDrawElements(GL_TRIANGLES, ships[ship_idx].ibo_size, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
}

static void draw_field(field& F) {
    glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Camera.m[0]);
    glUniformMatrix3fv(world_loc, 1, GL_TRUE, &F.move.m[0]);
    glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
    glUniform4f(f_color_loc, 0, 0.1, .6, 1);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer);
    glDrawElements(GL_TRIANGLES, ibo_size, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
}


static void draw_bombs(field& F) {
    for (int i = 0; i < (int)F.bombs.size(); i++) {
        draw_cell(F.bombs[i], bomb_color, F);
    }

}


static void RenderSceneCB()
{
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
    if (curr_ship != -1) {
        draw_ship(curr_ship, current_ship_color);
    }
    draw_bombs(field1);
    draw_bombs(field2);
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
    /*
    for (int i = 0; i < 14 * amount_of_polygons; i += 2) {
        cout << '(' << points[i] << "; " << points[i + 1] << ')' << endl;
    }
    */
    for (int i = 0; i < (int)ibo.size(); i++) {
        ibo_data[i] = ibo[i];
    }
    glGenBuffers(1, &ibo_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibo_data), ibo_data, GL_STATIC_DRAW);
    ibo_size = ibo.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ships = new ship[10];
    amount_of_ships = 10;
    ships[0].power(4);
    ships[0].pos.m[2] = 500;
    ships[0].pos.m[5] = 300;
    for (int i = 1; i < 3; i++) {
        ships[i].power(3);
        ships[i].pos.m[2] = 600;
        ships[i].pos.m[5] = 300 + (i - 1) * 180;
    }

    for (int i = 3; i < 6; i++) {
        ships[i].power(2);
        ships[i].pos.m[2] = 700;
        ships[i].pos.m[5] = 300 + (i - 3) * 120;
    }
    for (int i = 6; i < 10; i++) {
        ships[i].power(1);
        ships[i].pos.m[2] = 800;
        ships[i].pos.m[5] = 300 + (i - 6) * 60;
    }
    //glutSetCursor(0, 0);
}

void init_fields() {
    field1.move.m[2] = field1.move.m[5] = field2.move.m[5] = 40;
    field2.move.m[2] = 860;
    field1.bombs.push_back(12);
}


int main(int argc, char** argv)
{
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
    return 0;
}

