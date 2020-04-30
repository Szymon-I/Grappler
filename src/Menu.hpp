enum
{
    CAMERA_OPTIONS,
    EXIT,
};
enum
{
    FREE_CAMERA,
    THIRD_PERSON,
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
        break;
    case THIRD_PERSON:
        printf("THIRD_PERSON\n");
    }
}

void init_menu()
{
    int camera_menu = glutCreateMenu(CameraMenu);
    glutAddMenuEntry("third person", THIRD_PERSON);
    glutAddMenuEntry("free camera", FREE_CAMERA);

    int main_menu = glutCreateMenu(Menu);
    glutAddSubMenu("change camera", camera_menu);
    glutAddMenuEntry("exit", EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}