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
#include "Box.hpp"
#include "Game.hpp"

#define MONKEY_N 4
#define FLOWER_N 100
#define M_PI 3.14159265358979323846
#define BOX_N 5
#define CEMENT_N 3
#define CABINET_N 3
#define TREE_N 6
// delay in ms for applying gravitation
#define GRAVITATION_TIME 17
// Global variables
bool serial_attached = false;
float grappler_y = 3.0;
// Global camera
Camera camera;

// matrices for diplaying content
glm::mat4x4 Matrix_proj;    // projection matrix
glm::mat4x4 Matrix_mv;      // modelview matrix
glm::mat4x4 Matrix_proj_mv; // projection * modelview matrix

// all objects
ProgramHandler box_programs[BOX_N];
ProgramHandler warehouse_program;
ProgramHandler roof_program;
ProgramHandler sky_program1;
ProgramHandler hook_program;
ProgramHandler ground_program;
ProgramHandler cements[CEMENT_N];
ProgramHandler cabinets[CABINET_N];
ProgramHandler trees[TREE_N];

vector<Box *> Boxes;
// all objects in list format
vector<ProgramHandler *> AllPrograms;

// Serial handler
Serial serial;

// Grappler objects
Grappler grappler;

// Global light cointaining all lights on scene
Light global_light;

// Texts displayed on screen
Text textFPS;
Text textFPSCAP;

Game *game;

// FPS
float fps = 0.0;

// function prototypes
void show_fps();
void mod_mv();
void display_boxes();
void display_scene();
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

    // display objects
    display_scene();
    grappler.display_grappler(Matrix_proj, Matrix_mv, camera);
    display_boxes();
    show_fps();
    game->display(Matrix_proj, Matrix_mv);

    // swap buffer with the new generated one
    glutSwapBuffers();
}
void display_boxes()
{
    for (Box *box : Boxes)
    {
        box->display(grappler.get_position(), Matrix_proj, Matrix_mv);
    }
}
void display_scene()
{
    for (int i = 0; i < CEMENT_N; i++)
    {
        cements[i].display(Matrix_proj, Matrix_mv);
    }
    for (int i = 0; i < CABINET_N; i++)
    {
        cabinets[i].display(Matrix_proj, Matrix_mv);
    }
    for (int i = 0; i < TREE_N; i++)
    {
        trees[i].display(Matrix_proj, Matrix_mv);
    }

    warehouse_program.display(Matrix_proj, Matrix_mv);
    roof_program.display(Matrix_proj, Matrix_mv);
    sky_program1.display(Matrix_proj, Matrix_mv);
    ground_program.display(Matrix_proj, Matrix_mv);

    // iterating allprograms will overrwrite boxes with collision (texture bug)
    // for (ProgramHandler *p : AllPrograms)
    // {
    //     p->display(Matrix_proj, Matrix_mv);
    // }
}

void show_fps()
{
    textFPSCAP.RenderText("FPS CAP: 59.9", 10, glutGet(GLUT_WINDOW_HEIGHT) - 30, 0.8f, glm::vec3(0.0, 1.0f, 0.0f));
    char str[10];
    snprintf(str, sizeof(str), "%.1f", fps);
    std::string s(str);
    textFPS.RenderText("FPS: " + s, 10, glutGet(GLUT_WINDOW_HEIGHT) - 70, 0.8f, glm::vec3(1.0, 0.0f, 0.0f));
}
// change window size
void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    camera.set_window_dimenstions(width, height);
    Matrix_proj = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 1000.0f);
    textFPSCAP.InitText((char *)"libs/FFF_Tusj.ttf", 36);
    textFPS.InitText((char *)"libs/arial.ttf", 36);
    game->reshape_text();
}
void init_decorations()
{
    glm::vec3 cement_locations[CEMENT_N] = {
        glm::vec3(15.0f, 0.0f, -6.0f),
        glm::vec3(30.0f, 0.0f, 8.0f),
        glm::vec3(-9.0f, 0.0f, 13.0f),
    };
    for (int i = 0; i < CEMENT_N; i++)
    {
        cements[i].init("objects/cement.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/cement.png", global_light, Material::BlackRubber, false);
        cements[i].set_translate(cement_locations[i]);
        cements[i].set_scale(glm::vec3(0.05, 0.05, 0.05));
        AllPrograms.push_back(&cements[i]);
    }

    glm::vec3 cabinet_locations[CABINET_N] = {
        glm::vec3(40.0f, 0.0f, -10.0f),
        glm::vec3(-40.0f, 0.0f, -10.0f),
        glm::vec3(-10.0f, 0.0f, -10.0f)};
    for (int i = 0; i < CABINET_N; i++)
    {
        cabinets[i].init("objects/cabinet.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/metal.png", global_light, Material::BlackRubber, false);
        cabinets[i].set_translate(cabinet_locations[i]);
        cabinets[i].set_scale(glm::vec3(0.05, 0.05, 0.05));
        AllPrograms.push_back(&cabinets[i]);
    }
    glm::vec3 tree_locations[TREE_N] = {
        glm::vec3(-40.0f, 0.0f, -40.0f),
        glm::vec3(-60.0f, 0.0f, 40.0f),
        glm::vec3(0.0f, 0.0f, -40.0f),
        glm::vec3(0.0f, 0.0f, 50.0f),
        glm::vec3(40.0f, 0.0f, 60.0f),
        glm::vec3(60.0f, 0.0f, -50.0f)};
    for (int i = 0; i < TREE_N; i++)
    {
        trees[i].init("objects/tree.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/tree.png", global_light, Material::BlackRubber, false);
        trees[i].set_translate(tree_locations[i]);
        AllPrograms.push_back(&trees[i]);
    }
}
void init_boxes()
{
    glm::vec3 box_locations[BOX_N] = {
        glm::vec3(20.0f, 0.0f, 20.0f),
        glm::vec3(20.0f, 0.0f, 0.0f),
        glm::vec3(-20.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 20.0f),
        glm::vec3(0.0f, 0.0f, 10.0f)};
    for (int i = 0; i < BOX_N; i++)
    {
        box_programs[i].init("objects/box.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/box.png", global_light, Material::WhiteRubber);
        box_programs[i].set_translate(box_locations[i]);
        box_programs[i].load_second_tex("textures/box2.png");
        AllPrograms.push_back(&box_programs[i]);
        Boxes.push_back(new Box(&box_programs[i]));
    }
}
// initializations of all objects
void Initialize()
{
    // clear buffer with black color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // initialize global light
    std::vector<glm::vec3> ambient = {glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.3, 0.3, 0.3)};
    std::vector<glm::vec3> diffuse = {glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0)};
    std::vector<glm::vec3> position = {glm::vec3(0.0, 1.0, -10.0), glm::vec3(0.0, 1.0, -10.0), glm::vec3(8.0, 1.0, -10.0)};
    global_light.init(ambient, diffuse, position);

    // initialize all objects
    warehouse_program.init("objects/warehouse.obj", "shaders/vertex_ground.glsl", "shaders/fragment.glsl", "textures/metal2.png", global_light, Material::Brass, false);
    warehouse_program.set_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    AllPrograms.push_back(&warehouse_program);

    roof_program.init("objects/roof.obj", "shaders/vertex_ground.glsl", "shaders/fragment.glsl", "textures/ground.png", global_light, Material::Brass, false);
    roof_program.set_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    AllPrograms.push_back(&roof_program);

    ground_program.init("objects/ground.obj", "shaders/vertex_ground.glsl", "shaders/fragment.glsl", "textures/tree.png", global_light, Material::Brass, false);
    ground_program.set_translate(glm::vec3(0.0f, -0.05f, 0.0f));
    ground_program.set_scale(glm::vec3(2.0f, 2.0f, 2.0f));
    AllPrograms.push_back(&ground_program);

    sky_program1.init("objects/sky.obj", "shaders/vertex.glsl", "shaders/fragment_sky.glsl", "textures/sky.png", global_light, Material::Tin, false);
    sky_program1.set_scale(glm::vec3(40.0, 40.0, 40.0));
    AllPrograms.push_back(&sky_program1);

    hook_program.init("objects/hook.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/hook.png", global_light, Material::Tin);
    hook_program.set_scale(glm::vec3(0.5f, 0.5f, 0.5f));
    //AllPrograms.push_back(&hook_program);
    // use hook as a grappler object
    grappler.init(hook_program, 0.2);
    // set starting position
    grappler.set_position(glm::vec3(0.0f, Y_OFFSET, 0.0f));

    init_boxes();
    init_decorations();

    ProgramHandler *game_box = new ProgramHandler();
    game_box->init("objects/platform.obj", "shaders/vertex.glsl", "shaders/fragment.glsl", "textures/tree.png", global_light, Material::WhiteRubber);
    game = new Game(EASY, game_box);
}
// clean all allocated data for objects
void clean(void)
{
    warehouse_program.clean();
    roof_program.clean();
    sky_program1.clean();
    hook_program.clean();
    for (Box *box : Boxes)
    {
        delete box;
    }
    delete game;
}

// handle uart interrupt
void getSerialHandler()
{
    serial.handler();
    if (serial.is_ready())
    {
        grappler.move_grappler(serial.pass_message(), AllPrograms);
    }
}
// measure time elapsed between rendered views
void measureFps(void)
{
    static clock_t last_frame = 0.0;
    clock_t this_frame = clock();

    fps = (CLOCKS_PER_SEC / ((float)(this_frame - last_frame)));

    last_frame = this_frame;
}
// make gravitation step for all boxes
void gravitation(int)
{
    bool applied = false;
    for (Box *box : Boxes)
    {
        applied |= box->update_gravitation();
        if (!box->is_falling() && box->is_inside_field(game->get_active_field()) && !box->is_grabbed())
        {
            game->step();
        }
    }
    if (applied)
    {
        glutPostRedisplay();
    }
    glutTimerFunc(GRAVITATION_TIME, gravitation, 0);
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
    }
    glutIdleFunc(getSerialHandler);
    glutDisplayFunc(DisplayScene);
    glutReshapeFunc(Reshape);
    glutMouseFunc(MouseButton);
    //glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MouseMotion);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeys);
    glutTimerFunc(17, timer, 0);
    glutTimerFunc(GRAVITATION_TIME, gravitation, 0);
    glutMainLoop();

    clean();

    return 0;
}