#include <GL/glew.h>
#include <GL/freeglut.h>
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
using namespace std;
unsigned int vbo, ibo_buffer, program;
unsigned int f_color_loc, world_loc, coord_loc, angle_loc;
polygon* Field;
Matrix3f World;
int amount_of_polygons;
int ibo_size;
ship* ships;
int amount_of_ships;
int curr_ship;
float matrixes[6][4];
void init_matrixes() {
    for (int i = 0; i < 6; i++) {
        matrixes[i][0] = matrixes[i][3] = cosf(i * (M_PI / 3));
        matrixes[i][1] = -sinf(i * (M_PI / 3));
        matrixes[i][2] = -matrixes[i][1];
    }
}

void PressEvent(unsigned char key, int x, int y) {
    if (key == 'w') {
        ships[curr_ship].pos.m[5] += D_X;
    } else if (key == 's') {
        ships[curr_ship].pos.m[5] -= D_X;
    } else if (key == 'a') {
        ships[curr_ship].pos.m[2] -= D_X; 
    } else if (key == 'd') {
        ships[curr_ship].pos.m[2] += D_X;
    } else if (key == 'c') {
        y = 768 - y;
        for (int i = 0; i < amount_of_polygons; i++) {
            if (in_polygon(point(x, y), Field[i])) {
                x = Field[i].centre.x;
                y = Field[i].centre.y;
            }
        }
        ships[curr_ship].pos.m[2] = x;
        ships[curr_ship].pos.m[5] = y;
    }

}

void MouseEvent(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON and state == GLUT_UP) {
        y = 768 - y;
        for (int i = 0; i < amount_of_polygons; i++) {
            if (in_polygon(point(x, y), Field[i])) {
                x = Field[i].centre.x;
                y = Field[i].centre.y;
            }
        }
        ships[curr_ship].pos.m[2] = x;
        ships[curr_ship].pos.m[5] = y;
    
    }
}

void PressSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
        curr_ship ^= 1;
    } else if (key == GLUT_KEY_LEFT) {
        curr_ship ^= 1;
    } else if (key == GLUT_KEY_UP) {
        ships[curr_ship].rotate();
    }
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

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.5, 0.0, 1);   
    glUniformMatrix3fv(world_loc, 1, GL_TRUE, &World.m[0]);
    glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
    glUniform4f(f_color_loc, 0, 0, 1, 1);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer);
    glDrawElements(GL_TRIANGLES, ibo_size, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    
    for (int i = 0; i < amount_of_ships; i++) {
        glUniformMatrix3fv(world_loc, 1, GL_TRUE, &ships[i].pos.m[0]);
        glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[ships[i].rot][0]);
        glUniform4f(f_color_loc, 1, 0.5, 1, 1);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
        glDrawElements(GL_TRIANGLES, ships[i].ibo_size, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

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
//    glutPassiveMotionFunc(Pointer);
}


void init_resourses()
{
    GLuint vs = create_shader("shader/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fs = create_shader("shader/fragment.glsl", GL_FRAGMENT_SHADER);
    program = create_program(vs, fs);
    glUseProgram(program);
    world_loc = glGetUniformLocation(program, "World");
    f_color_loc = glGetUniformLocation(program, "f_color");
    angle_loc = glGetUniformLocation(program, "rotate");

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
    ships = new ship[2];
    amount_of_ships = 2;
    ships[1].power(4);
}

int main(int argc, char** argv)
{
    Field = gen_field(10, 10);
    amount_of_polygons = 100;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1366, 768);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Tutorial 01");
    InitializeGlutCallbacks();
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }
    glClearColor(0.0f, .5f, .0f, .0f);
    init_matrixes();
    init_ship_object();

    CreateVertexBuffer();
    cout << "created" << endl;
    init_resourses();
    glutMainLoop();
    return 0;
}

