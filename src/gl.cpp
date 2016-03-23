#include <geom.h>
#include <math_3d.h>
#include <polygon.h>
#include <field.h>
#include <cstdio>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <file.h>
#include <shader.h>
using namespace std;
GLuint vbo, ibo_buffer, small_vbo, small_ibo_buffer, gWorld;
polygon* Field;
Matrix3f World;
int amount_of_polygons;
int ibo_size;

void PressEvent(unsigned char key, int x, int y) {
    World.m[2] += 100;
    cout << World.m[2] << endl;
}

void PressSpecial(int key, int x, int y) {
}

void to_float(float* arr, int& idx, point c)
{
    arr[idx] = c.x;
    arr[idx + 1] = c.y;
    idx += 2;
}
void draw_polygon(polygon& p, float* vbo_data, int idx, vector<int>& ibo)
{
    to_float(vbo_data, idx, p.centre);
    int was_idx = idx;
   
    for (int i = 0; i < (int)p.points.size(); i++)
    {
        to_float(vbo_data, idx, p.points[i]);
    }
    idx = was_idx / 2;
    for (int i = 0; i < (int)p.points.size(); i++)
    {
        ibo.push_back(idx - 1);
        ibo.push_back(idx + i);
        ibo.push_back(idx + (i + 1) % p.points.size());
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
    glClear(GL_COLOR_BUFFER_BIT);
//    draw_hex(0);
//    /*
    glClearColor(0.0, 0.5, 0.0, 1);   
    glUniformMatrix3fv(gWorld, 1, GL_FALSE, &World.m[0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer);
    glDrawElements(GL_TRIANGLES, ibo_size, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
//    */
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
//    glutPassiveMotionFunc(Pointer);
}


void init_resourses()
{
    GLuint vs = create_shader("shader/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fs = create_shader("shader/fragment.glsl", GL_FRAGMENT_SHADER);
    GLuint program = create_program(vs, fs);
    glUseProgram(program);
    gWorld = glGetUniformLocation(program, "World");
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
    float small[14];
    ibo.clear();
    draw_polygon(Field[0], small, 0, ibo);
    glGenBuffers(1, &small_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, small_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    unsigned int small_ibo_data[ibo.size()];
    /*
    for (int i = 0; i < 14 * amount_of_polygons; i += 2) {
        cout << '(' << points[i] << "; " << points[i + 1] << ')' << endl;
    }
    */
    for (int i = 0; i < (int)ibo.size(); i++) {
        small_ibo_data[i] = ibo[i];
    }
    glGenBuffers(1, &small_ibo_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, small_ibo_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(small_ibo_data), small_ibo_data, GL_STATIC_DRAW);

/*
    float points[] = {0, -0.5, -0.5, 0, 0, 0.5, 0.5, 0};
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    unsigned int ibo_data[] = {0, 1, 2, 0, 2, 3};
    glGenBuffers(1, &ibo_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibo_data), ibo_data, GL_STATIC_DRAW);
    */
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

    CreateVertexBuffer();
    init_resourses();
    glutMainLoop();
    return 0;
}

