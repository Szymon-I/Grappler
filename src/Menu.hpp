enum
{
    CAMERA_OPTIONS,
    EXIT,
};
enum
{
    SERIAL_MONITOR_TURN_ON,
    SERIAL_MONITOR_TURN_OFF,
};

void Menu(int value)
{
    switch (value)
    {
    case EXIT:
        exit(0);
    }
}

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
    }
    glutPostRedisplay();
}

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

void init_menu()
{
    int camera_menu = glutCreateMenu(CameraMenu);
    glutAddMenuEntry("third person", THIRD_PERSON);
    glutAddMenuEntry("free camera", FREE_CAMERA);

    int serial_menu = glutCreateMenu(SerialMenu);
    glutAddMenuEntry("turn on monitor", SERIAL_MONITOR_TURN_ON);
    glutAddMenuEntry("turn off monitor", SERIAL_MONITOR_TURN_OFF);

    int main_menu = glutCreateMenu(Menu);
    glutAddSubMenu("change camera", camera_menu);
    glutAddSubMenu("serial", serial_menu);

    glutAddMenuEntry("exit", EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}