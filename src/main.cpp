#include <stdio.h>
#include <time.h>
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
#include "Menu.hpp"
#include "Material.hpp"
#include "Text.hpp"

#define MONKEY_N 4
#define TREE_N 3
#define FLOWER_N 100
#define M_PI 3.14159265358979323846
#define PACKET_DIVISOR 4
// Global variables
bool serial_attached = false;
// Global camera
Camera camera;

// matrices for diplaying content
glm::mat4x4 Matrix_proj;    // projection matrix
glm::mat4x4 Matrix_mv;      // modelview matrix
glm::mat4x4 Matrix_proj_mv; // projection * modelview matrix

// all objects
ProgramHandler wolf_program;
ProgramHandler ground_program;
ProgramHandler sky_program1;
ProgramHandler monkey_programs[MONKEY_N];
ProgramHandler tree_programs[TREE_N];
ProgramHandler flower_program[FLOWER_N];
ProgramHandler virus_program;
ProgramHandler hook_program;

// all objects in list format
vector<ProgramHandler> AllPrograms;

// Serial handler
Serial serial;

// Grappler objects
Grappler grappler;

// Global light cointaining all lights on scene
Light global_light;

// Texts displayed on screen
Text textFPS;
Text textFPSCAP;

// FPS
float fps = 0.0;

// function prototypes
void monkey_circle();
void mod_mv();
void sow_trees();
void sow_flowers();

// main function for displaying all content
void DisplayScene()
{
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // re-initialize Matrix_mv for new display
    Matrix_mv = glm::mat4x4(1.0f);

    // move camera if mouse is outside window
    camera.mouse_edge();
    // apply all camera modifications to global Matrix_mv made by camera
    Matrix_mv = camera.apply_camera(Matrix_mv, grappler.get_position());

    //movement is handled inside getSerialHandler
    grappler.display_grappler(Matrix_proj, Matrix_mv, camera);

    // diplay ground
    ground_program.display(Matrix_proj, Matrix_mv);

    // display sky objects
    sky_program1.set_scale(glm::vec3(10.0, 10.0, 10.0));
    sky_program1.display(Matrix_proj, Matrix_mv);

    // diplay virus
    virus_program.set_rotation(glm::vec3(0.0, 1.0, 0.0), 0);
    virus_program.set_rotation_animation(0.02);
    virus_program.set_translate(glm::vec3(-6.0f, 0.5f, 6.0f));
    virus_program.display(Matrix_proj, Matrix_mv);

    wolf_program.set_translate(glm::vec3(3.0f, 0.0f, 0.0f));
    wolf_program.display(Matrix_proj, Matrix_mv);

    // display other objects in loops
    sow_flowers();
    sow_trees();
    monkey_circle();

    textFPSCAP.RenderText("FPS CAP: 59.9", 10, glutGet(GLUT_WINDOW_HEIGHT) - 30, 0.8f, glm::vec3(0.0, 1.0f, 0.0f));
    char str[10];
    snprintf(str, sizeof(str), "%.1f", fps);
    std::string s(str);
    textFPS.RenderText("FPS: " + s, 10, glutGet(GLUT_WINDOW_HEIGHT) - 70, 0.8f, glm::vec3(1.0, 0.0f, 0.0f));

    // swap buffer with the new generated one
    glutSwapBuffers();
}

// display all flowers
void sow_flowers(void)
{
    for (int i = 0; i < FLOWER_N; i++)
    {
        flower_program[i].set_translate(glm::vec3(i % 10, 0.0f, i / 10 + 6.0));
        flower_program[i].set_scale(glm::vec3(0.5, 0.5, 0.5));
        flower_program[i].display(Matrix_proj, Matrix_mv);
    }
}
// display all trees
void sow_trees()
{

    tree_programs[0].set_translate(glm::vec3(15.0f, 0.0f, 0.0f));
    tree_programs[1].set_translate(glm::vec3(20.0f, 0.0f, 10.0f));
    tree_programs[2].set_translate(glm::vec3(-10.0f, 0.0f, -10.0f));

    for (int i = 0; i < TREE_N; i++)
    {
        tree_programs[i].set_scale(glm::vec3(0.25f * (i + 1), 0.25f * (i + 1), 0.25f * (i + 1)));
        tree_programs[i].display(Matrix_proj, Matrix_mv);
    }
}
// display all monkeys
void monkey_circle()
{
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
// change window size
void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    camera.set_window_dimenstions(width, height);
    Matrix_proj = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 1000.0f);
    textFPSCAP.InitText((char *)"libs/FFF_Tusj.ttf", 36);
    textFPS.InitText((char *)"libs/arial.ttf", 36);
}
// initializations of all objects
void Initialize()
{
    // clear buffer with black color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // initialize global light
    std::vector<glm::vec3> ambient = {glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.3, 0.3, 0.3)};
    std::vector<glm::vec3> diffuse = {glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0)};
    std::vector<glm::vec3> position = {glm::vec3(0.0, 1.0, 8.0), glm::vec3(0.0, 1.0, -8.0), glm::vec3(8.0, 1.0, 0.0)};
    global_light.init(ambient, diffuse, position);

    // initialize all objects
    ground_program.init("objects/ground.obj", "shaders/vertex_ground.glsl", "shaders/fragment.glsl", "textures/ground.png", global_light, Material::Brass, false);
    sky_program1.init("objects/sky.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/sky.png", global_light, Material::Tin, false);
    virus_program.init("objects/virus.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/virus.png", global_light, Material::Tin, false);

    hook_program.init("objects/hook.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/hook.png", global_light, Material::Tin);
    hook_program.set_scale(glm::vec3(0.5f, 0.5f, 0.5f));

    // use hook as a grappler object
    grappler.init(hook_program, 0.2);
    // set starting position
    grappler.set_position(glm::vec3(0.0f, Y_OFFSET, 0.0f));

    for (int i = 0; i < TREE_N; i++)
    {
        tree_programs[i].init("objects/tree.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/tree.png", global_light, Material::Emerald, false);
        AllPrograms.push_back(tree_programs[i]);
    }

    for (int i = 0; i < MONKEY_N; i++)
    {
        monkey_programs[i].init("objects/monkey.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/monkey.png", global_light, Material::BlackRubber, false);
        AllPrograms.push_back(monkey_programs[i]);
    }
    for (int i = 0; i < FLOWER_N; i++)
    {
        flower_program[i].init("objects/flower.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/flower.png", global_light, Material::Emerald, false);
        AllPrograms.push_back(flower_program[i]);
    }

    wolf_program.init("objects/wolf.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/wolf.png", global_light, Material::WhiteRubber);

    // add all remaining objects/programs to global list
    AllPrograms.push_back(ground_program);
    AllPrograms.push_back(sky_program1);
    AllPrograms.push_back(virus_program);
    AllPrograms.push_back(wolf_program);
    AllPrograms.push_back(hook_program);
}
// clean all allocated data for objects
void clean(void)
{
    wolf_program.clean();
    ground_program.clean();
    sky_program1.clean();
    virus_program.clean();
    hook_program.clean();

    for (int i = 0; i < FLOWER_N; i++)
        flower_program[i].clean();

    for (int i = 0; i < TREE_N; i++)
        tree_programs[i].clean();

    for (int i = 0; i < MONKEY_N; i++)
        monkey_programs[i].clean();
}

// handle uart interrupt
void getSerialHandler()
{
    static int packet_counter = 0;
    serial.handler();

    if (serial.is_ready() && packet_counter >= PACKET_DIVISOR)
    {

        grappler.move_grappler(serial.pass_message(), AllPrograms);
        packet_counter = 0;
    }
    packet_counter++;
}

void measureFps(void)
{
    static clock_t last_frame = 0.0;
    clock_t this_frame = clock();

    fps = (CLOCKS_PER_SEC / ((float)(this_frame - last_frame)));

    last_frame = this_frame;
}

// timer for rendering view in const fps
void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(17, timer, 0);
    measureFps();
}

// main function
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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

    Initialize();
    // init serial with two main arguments
    if (argc >= 3)
    {
        serial_attached = true;
        serial.init(argv[1], atoi(argv[2]));
        glutIdleFunc(getSerialHandler);
    }
    glutDisplayFunc(DisplayScene);
    glutReshapeFunc(Reshape);
    glutMouseFunc(MouseButton);
    //glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MouseMotion);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeys);
    glutTimerFunc(17, timer, 0);

    glutMainLoop();

    // Cleaning();
    //clean();

    return 0;
}