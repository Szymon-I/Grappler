#pragma once
// shaders paths
#define FRAGMENT_BASIC_PATH "./shaders/fragment.glsl"
#define FRAGMENT_DIRECTIONAL_PATH "./shaders/fragment_directional.glsl"
#define FRAGMENT_SPOTLIGHT_PATH "./shaders/fragment_spotlight.glsl"
// main menu options
enum
{
    CAMERA_OPTIONS,
    SHADERS,
    EXIT,
};
// serial menu options
enum
{
    SERIAL_MONITOR_TURN_ON,
    SERIAL_MONITOR_TURN_OFF,
};
// shader menu options
enum
{
    FRAGMENT_BASIC,
    FRAGMENT_DIRECTIONAL,
    FRAGMENT_SPOTLIGHT
};

// main menu
void Menu(int value)
{
    switch (value)
    {
    case EXIT:
        exit(0);
    }
}
// apply new shaders for every program
void applyShaders(std::string shader_path)
{
    for (ProgramHandler program : AllPrograms)
    {
        program.update_fragment_shader(shader_path);
    };
}
// camera menu
void CameraMenu(int value)
{
    switch (value)
    {
    case FREE_CAMERA:
        printf("FREE_CAMERA\n");
        camera.set_mode(FREE_CAMERA, grappler.get_position());
        break;
    case THIRD_PERSON:
        printf("THIRD_PERSON\n");
        camera.set_mode(THIRD_PERSON, grappler.get_position());
        break;
    }
}
// shader menu
void ShaderMenu(int value)
{
    switch (value)
    {
    case FRAGMENT_BASIC:
        printf("FRAGMENT_BASIC\n");
        applyShaders(FRAGMENT_BASIC_PATH);
        break;
    case FRAGMENT_DIRECTIONAL:
        printf("FRAGMENT_DIRECTIONAL\n");
        applyShaders(FRAGMENT_DIRECTIONAL_PATH);
        break;
    case FRAGMENT_SPOTLIGHT:
        printf("FRAGMENT_SPOTLIGHT\n");
        applyShaders(FRAGMENT_SPOTLIGHT_PATH);
        break;
    }
}
// serial menu
void SerialMenu(int value)
{
    switch (value)
    {
    case SERIAL_MONITOR_TURN_ON:
        printf("SERIAL_MONITOR_TURN_ON\n");
        grappler.set_monitor(true);
        break;
    case SERIAL_MONITOR_TURN_OFF:
        printf("SERIAL_MONITOR_TURN_OFF\n");
        grappler.set_monitor(false);
    }
}
// initialize all menu options
void init_menu()
{
    int camera_menu = glutCreateMenu(CameraMenu);
    glutAddMenuEntry("third person", THIRD_PERSON);
    glutAddMenuEntry("free camera", FREE_CAMERA);

    int serial_menu = glutCreateMenu(SerialMenu);
    glutAddMenuEntry("turn on monitor", SERIAL_MONITOR_TURN_ON);
    glutAddMenuEntry("turn off monitor", SERIAL_MONITOR_TURN_OFF);

    int shader_menu = glutCreateMenu(ShaderMenu);
    glutAddMenuEntry("fragment basic", FRAGMENT_BASIC);
    glutAddMenuEntry("fragment directional", FRAGMENT_DIRECTIONAL);
    glutAddMenuEntry("fragment spotlight", FRAGMENT_SPOTLIGHT);

    int main_menu = glutCreateMenu(Menu);
    glutAddSubMenu("change camera", camera_menu);
    glutAddSubMenu("serial", serial_menu);
    glutAddSubMenu("shaders", shader_menu);

    glutAddMenuEntry("exit", EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}