enum
{
    CAMERA_OPTIONS,
    EXIT, // wyjście
} MAIN_MENU;
enum
{
    FREE_CAMERA,
    THIRD_PERSON, // wyjście
} CAMERA_MENU;

void Menu(int value)
{
    switch (value)
    {
    case CAMERA_OPTIONS:
        printf("test");
        break;
        // wyjście
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
        break;
    case THIRD_PERSON:
        printf("THIRD_PERSON\n");
    }
}

void init_menu()
{
    int camera_menu = glutCreateMenu(CameraMenu);
    glutAddMenuEntry("third_person", THIRD_PERSON);
    glutAddMenuEntry("free_camera", FREE_CAMERA);


    int main_menu = glutCreateMenu(Menu);
    glutAddSubMenu("camera_options", camera_menu);
    glutAddMenuEntry("Exit", EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}