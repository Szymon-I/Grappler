
// --------------------------------------------------------------
// Variables for control the view
#define VANISH_CHANGE 0.1
#include <GL/freeglut.h>
GLfloat _scene_rotate_x = 0.0f;
GLfloat _scene_rotate_y = 0.0f;
GLfloat _scene_translate_x = 0.0f;
GLfloat _scene_translate_y = 0.0f;
GLfloat _scene_translate_z = -3.0f;
int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;
extern float vanishing;

// --------------------------------------------------------------
void SpecialKeys( int key, int x, int y )
{
    switch( key )
    {

        case GLUT_KEY_LEFT:
            _scene_translate_x -= 0.1f;
            break;

        case GLUT_KEY_RIGHT:
            _scene_translate_x += 0.1f;
            break;

        case GLUT_KEY_UP:
            _scene_translate_y += 0.1f;
            break;

        case GLUT_KEY_DOWN:
            _scene_translate_y -= 0.1f;
            break;
    }

    glutPostRedisplay();
}
void vanish_info(float x){
	printf("Vanishing: %.1f\n",x);
}
// --------------------------------------------------------------
void Keyboard( unsigned char key, int x, int y )
{
    switch(key)
    {
		case 27:	// ESC key
			exit(0);
			break;

    }

}

// --------------------------------------------------------------
void MouseButton( int button, int state, int x, int y )
{
    
	if(button==3){
		_scene_translate_z += 0.1f;
	}
	if(button==4){
		_scene_translate_z -= 0.1f;
	}

	if( button == GLUT_LEFT_BUTTON )
    {

        _mouse_buttonState = state;


        if( state == GLUT_DOWN )
        {
            _mouse_buttonX = x;
            _mouse_buttonY = y;
        }
    }
	glutPostRedisplay();
}

// --------------------------------------------------------------
void MouseMotion( int x, int y )
{
    if( _mouse_buttonState == GLUT_DOWN )
    {
        _scene_rotate_y += 2*(x - _mouse_buttonX)/(float)glutGet( GLUT_WINDOW_WIDTH );
        _mouse_buttonX = x;
        _scene_rotate_x -= 2*(_mouse_buttonY - y)/(float)glutGet( GLUT_WINDOW_HEIGHT );
        _mouse_buttonY = y;
        glutPostRedisplay();
    }
}

// --------------------------------------------------------------
void MouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
        // Zoom in
        _scene_translate_z += 0.1f;
    }
    else
    {
        // Zoom out
        _scene_translate_z -= 0.1f;
    }

    glutPostRedisplay();
}