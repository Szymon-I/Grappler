#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "libs/SOIL.h"
#include "libs/serialib.h"
#include "libs/obj_loader.hpp"
#include "libs/shader_stuff.h"
#include "libs/texture_loader.hpp"

#include "Light.hpp"
#include "ProgramHandler.hpp"



#define MONKEY_N 4
#define TREE_N 3
#define M_PI 3.14159265358979323846
// ---------------------------------------
// Macierze przeksztalcen i rzutowania

glm::mat4x4 Matrix_proj;    // projection matrix
glm::mat4x4 Matrix_mv;      // modelview matrix
glm::mat4x4 Matrix_proj_mv; // projection*modelview matrix

ProgramHandler wolf_program;
ProgramHandler ground_program;
ProgramHandler sky_program1;
ProgramHandler sky_program2;
ProgramHandler monkey_programs[MONKEY_N];
ProgramHandler tree_programs[TREE_N];
Light global_light;

void monkey_circle();
void mod_mv();
void sow_trees();

// ---------------------------------------
void DisplayScene()
{
    // Czyszczenie ramki
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mod_mv();

    wolf_program.set_translate(glm::vec3(0.5f, 0.0f, 0.0f));
    wolf_program.set_scale(glm::vec3(1.5f, 1.5f, 1.5f));
    wolf_program.display(Matrix_proj, Matrix_mv);

    ground_program.set_scale(glm::vec3(1.5f, 1.5f, 1.5f));
    ground_program.display(Matrix_proj, Matrix_mv);

    sky_program1.set_scale(glm::vec3(10.0, 10.0, 10.0));
    sky_program1.display(Matrix_proj, Matrix_mv);

    sky_program2.set_scale(glm::vec3(10.0, 10.0, 10.0));
    sky_program2.set_rotation(glm::vec3(1.0, 0.0, 0.0), M_PI);
    sky_program2.display(Matrix_proj, Matrix_mv);

    sow_trees();
    monkey_circle();

    glutSwapBuffers();
}

void mod_mv()
{
    Matrix_mv = glm::mat4x4(1.0);
    Matrix_mv = glm::translate(Matrix_mv, glm::vec3(_scene_translate_x, _scene_translate_y, _scene_translate_z));
    Matrix_mv = glm::rotate(Matrix_mv, _scene_rotate_x, glm::vec3(1.0f, 0.0f, 0.0f));
    Matrix_mv = glm::rotate(Matrix_mv, _scene_rotate_y, glm::vec3(0.0f, 1.0f, 0.0f));
}

void sow_trees()
{

    tree_programs[0].set_translate(glm::vec3(3.0f, 0.0f, 0.0f));
    tree_programs[1].set_translate(glm::vec3(3.0f, 0.0f, 3.0f));
    tree_programs[2].set_translate(glm::vec3(-3.0f, 0.0f, -3.0f));

    for (int i = 0; i < TREE_N; i++)
    {
        tree_programs[i].set_scale(glm::vec3(0.25f * (i + 1), 0.25f * (i + 1), 0.25f * (i + 1)));
        tree_programs[i].display(Matrix_proj, Matrix_mv);
    }
}

void monkey_circle()
{
    monkey_programs[0].set_rotation(glm::vec3(0.0, 1.0, 0.0), 0 * M_PI / 2);
    monkey_programs[0].set_translate(glm::vec3(0.0f, 0.0f, -10.0f));

    monkey_programs[1].set_rotation(glm::vec3(0.0, 1.0, 0.0), 2 * M_PI / 2);
    monkey_programs[1].set_translate(glm::vec3(0.0f, 0.0f, 10.0f));

    monkey_programs[2].set_rotation(glm::vec3(0.0, 1.0, 0.0), 1 * M_PI / 2);
    monkey_programs[2].set_translate(glm::vec3(-10.0f, 0.0, 0.0f));

    monkey_programs[3].set_rotation(glm::vec3(0.0, 1.0, 0.0), -1 * M_PI / 2);
    monkey_programs[3].set_translate(glm::vec3(10.0f, 0.0, 0.0f));

    for (int i = 0; i < MONKEY_N; i++)
    {
        monkey_programs[i].set_scale(glm::vec3(2.0f, 2.0f, 2.0f));
        monkey_programs[i].display(Matrix_proj, Matrix_mv);
    }
}
// ---------------------------------------
void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    Matrix_proj = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 100.f);
}

// ---------------------------------------------------
void Initialize()
{
    _scene_translate_z = -10.0;
    _scene_rotate_x = 0.2f;
    _scene_rotate_y = -0.65f;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    global_light.init(glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 2.0, 0.0));

    wolf_program.init("objects/wolf.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/wolf.bmp", global_light);
    ground_program.init("objects/ground.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/ground.bmp", global_light);
    sky_program1.init("objects/sky.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/sky.bmp", global_light);
    sky_program2.init("objects/sky.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/sky.bmp", global_light);

    for (int i = 0; i < TREE_N; i++)
        tree_programs[i].init("objects/tree.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/tree.bmp", global_light);

    for (int i = 0; i < MONKEY_N; i++)
        monkey_programs[i].init("objects/monkey.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/monkey.bmp", global_light);
}

// ---------------------------------------------------
int main(int argc, char *argv[])
{
    // GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("OpenGL");

    // GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("GLEW Error\n");
        exit(1);
    }

    // OpenGL
    if (!GLEW_VERSION_3_2)
    {
        printf("Brak OpenGL 3.2!\n");
        exit(1);
    }

    Initialize();
    glutDisplayFunc(DisplayScene);
    glutReshapeFunc(Reshape);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutMouseWheelFunc(MouseWheel);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeys);

    glutMainLoop();

    return 0;
}