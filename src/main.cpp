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

#include "main.hpp"
#include "ProgramHandler.hpp"
#include "IOHandler.hpp"
#include "Serial.hpp"
#include "Grappler.hpp"
#include "Camera.hpp"
#include "Menu.hpp"

#define MONKEY_N 4
#define TREE_N 3
#define M_PI 3.14159265358979323846
// ---------------------------------------
// Macierze przeksztalcen i rzutowania

Camera camera;

glm::mat4x4 Matrix_proj;    // projection matrix
glm::mat4x4 Matrix_mv;      // modelview matrix
glm::mat4x4 Matrix_proj_mv; // projection*modelview matrix

ProgramHandler wolf_program;
ProgramHandler ground_program;
ProgramHandler sky_program1;
ProgramHandler sky_program2;
ProgramHandler monkey_programs[MONKEY_N];
ProgramHandler tree_programs[TREE_N];

Serial serial;
Grappler grappler;

Light global_light;

void monkey_circle();
void mod_mv();
void sow_trees();

// ---------------------------------------
void DisplayScene()
{
    // Czyszczenie ramki
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // apply all camera modifications to global Matrix_mv
    Matrix_mv = camera.apply_camera(Matrix_mv);
    camera.mouse_edge();

    //movement is handled inside getSerialHandler
    grappler.display_grappler(Matrix_proj, Matrix_mv);

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
    monkey_programs[0].set_rotation_animation(0.02);
    monkey_programs[0].set_rotation(glm::vec3(0.0, 1.0, 0.0), 0 * M_PI / 2);
    monkey_programs[0].set_translate(glm::vec3(0.0f, 0.0f, -10.0f));

    monkey_programs[1].set_rotation(glm::vec3(0.0, 1.0, 0.0), 2 * M_PI / 2);
    monkey_programs[1].set_translation_animation(0.01);
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
    camera.set_window_dimenstions(width, height);
    Matrix_proj = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 1000.0f);
}

// ---------------------------------------------------
void Initialize()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    std::vector<glm::vec3> ambient = {glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.3, 0.3, 0.3)};
    std::vector<glm::vec3> diffuse = {glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0)};
    std::vector<glm::vec3> position = {glm::vec3(0.0, 1.0, 8.0), glm::vec3(0.0, 1.0, -8.0), glm::vec3(8.0, 1.0, 0.0)};
    global_light.init(ambient, diffuse, position);

    ground_program.init("objects/ground2.obj", "shaders/vertex_ground.glsl", "shaders/fragment.glsl", "textures/ground.png", global_light, Material::Brass());
    sky_program1.init("objects/sky.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/sky.png", global_light, Material::Tin());
    sky_program2.init("objects/sky.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/sky.png", global_light, Material::Tin());

    for (int i = 0; i < TREE_N; i++)
        tree_programs[i].init("objects/tree.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/tree.png", global_light, Material::Emerald());

    for (int i = 0; i < MONKEY_N; i++)
        monkey_programs[i].init("objects/monkey.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/monkey.png", global_light, Material::BlackRubber());

    // grapler = wolf
    wolf_program.init("objects/wolf.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/wolf.png", global_light, Material::WhiteRubber());
    grappler.init(wolf_program, 0.2);
}

void clean(void)
{
    wolf_program.clean();
    ground_program.clean();
    sky_program1.clean();
    sky_program2.clean();

    for (int i = 0; i < TREE_N; i++)
        tree_programs[i].clean();

    for (int i = 0; i < MONKEY_N; i++)
        monkey_programs[i].clean();
}

// handle uart interrupt
void getSerialHandler()
{
    serial.handler();
    if (serial.is_ready())
    {
        grappler.move_grappler(serial.pass_message());
    }
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000.0/60.0, timer, 0);
}

// ---------------------------------------------------
int main(int argc, char *argv[])
{
    // GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    //glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    glutCreateWindow("Grappler");
    init_menu();
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

    // init serial with main arguments
    serial.init(argv[1], atoi(argv[2]));

    Initialize();
    glutIdleFunc(getSerialHandler);
    glutDisplayFunc(DisplayScene);
    glutReshapeFunc(Reshape);
    glutMouseFunc(MouseButton);
    //glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MouseMotion);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeys);
    glutTimerFunc(1000.0/60.0, timer, 0);

    glutMainLoop();

    // Cleaning();
    clean();

    return 0;
}