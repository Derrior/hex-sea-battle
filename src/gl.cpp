#include <geom.h>
#include <polygon.h>
#include <field.h>
#include <cstdio>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <file.h>

using namespace std;
GLuint vbo;
typedef float* vec3f;
GLuint create_shader(const char *filename, GLenum type) {
    GLchar *source = read_file(filename);
    if (source == NULL) {
        fprintf(stderr, "Error while compiling shader %s: source cannot be read.\n", filename);
        return -1;
    }
    GLuint shader = glCreateShader(type);
    const GLchar *sources[2] = {
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"
        "#define GLES2\n",
#else
        "#version 120\n",
#endif
        source
    };
    glShaderSource(shader, 2, sources, NULL);
    free(source);
    glCompileShader(shader);
    GLint compile_status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        fprintf(stderr, "Error while compiling shader %s:\n", filename);
        glDeleteShader(shader);
        return -1;
    }
    return shader;
}

GLuint create_program(GLuint vs, GLuint fs) {
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    GLint link_status = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        fprintf(stderr, "Error while linking program:\n");
        glDeleteProgram(prog);
        return -1;
    }
    return prog;
}
static void RenderSceneCB()
{
    
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glutSwapBuffers();
}
        
static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
//    glutIdleFunc(RenderSceneCB);
}


void init_resourses()
{
    GLuint vs = create_shader("shader/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fs = create_shader("shader/fragment.glsl", GL_FRAGMENT_SHADER);
    GLuint program = create_program(vs, fs);
    glUseProgram(program);
}
static void CreateVertexBuffer()
{
    vec3f points[3];
    points[0] = new float[2];
    points[1] = new float[2];
    points[2] = new float[2];
    points[0][0] = 0.0f;
    points[0][1] = 0.0f;
    points[1][0] = 0.5f;
    points[1][1] = 0.0f;
    points[2][0] = 0.0f;
    points[2][1] = 0.5f;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 01");
    InitializeGlutCallbacks();
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }
    glClearColor(0.0f, 1.0f, .0f, .0f);

    CreateVertexBuffer();
    init_resourses();
    glutMainLoop();
    return 0;
}

