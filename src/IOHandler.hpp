
// --------------------------------------------------------------
// Variables for control the view
#include <GL/freeglut.h>
#include <stdio.h>

#define VANISH_CHANGE 0.1

GLfloat _scene_rotate_x = 0.0f;
GLfloat _scene_rotate_y = 0.0f;
GLfloat _scene_translate_x = 0.0f;
GLfloat _scene_translate_y = 0.0f;
GLfloat _scene_translate_z = -1.0f;

// angle of rotation for the camera direction
GLfloat angle = 0.0;
// actual vector representing the camera’s direction
GLfloat lx = 1.0f, lz = -1.0f, ly = 0.0f;
// XZ position of the camera
GLfloat xx = 0.0f, zz = -1.0f, yy = 1.0f;
// mouse variables
GLfloat mouseOnEdgeSpeed = 0.0002, fraction = 0.2f;
int oldMouseX = 960, oldMouseY = 540;

int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;
extern float vanishing;

// --------------------------------------------------------------
void SpecialKeys( int key, int x, int y )
{
    switch( key )
    {

        case GLUT_KEY_LEFT:
            //_scene_translate_x -= 0.1f;
            break;

        case GLUT_KEY_RIGHT:
            //_scene_translate_x += 0.1f;
            break;

        case GLUT_KEY_UP:
            //_scene_translate_y += 0.1f;
            break;

        case GLUT_KEY_DOWN:
            //_scene_translate_y -= 0.1f;
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
	switch (key) {
	case 'a':
		xx -= cos(angle);
		zz -= sin(angle);
		break;
	case 'd':
		xx += cos(angle);
		zz += sin(angle);
		break;
	case 'w':
		xx += lx * fraction;
		zz += lz * fraction;
		yy += ly * fraction;
		break;
	case 's':
		xx -= lx * fraction;
		zz -= lz * fraction;
		yy -= ly * fraction;
		break;
	case 27:
		exit(0);
		break;
	}

    glutPostRedisplay();
}

// --------------------------------------------------------------
void MouseButton( int button, int state, int x, int y )
{

	if(button==3){
		//_scene_translate_z += 0.1f;
	}
	if(button==4){
		//_scene_translate_z -= 0.1f;
	}

	if( button == GLUT_LEFT_BUTTON )
    {

        _mouse_buttonState = state;


        if( state == GLUT_DOWN )
        {
            //_mouse_buttonX = x;
            //_mouse_buttonY = y;
        }
    }
	glutPostRedisplay();
}

// --------------------------------------------------------------
void MouseMotion( int x, int y )
{
	angle += (x - oldMouseX) / 100.0;
	ly -= (y - oldMouseY) / 100.0;

	lx = sin(angle);
	lz = -cos(angle);

	oldMouseX = x;
	oldMouseY = y;
    glutPostRedisplay();

}

// --------------------------------------------------------------
void MouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
	{
		xx += lx * fraction;
		zz += lz * fraction;
		yy += ly * fraction;
	}
	else
	{
		xx -= lx * fraction;
		zz -= lz * fraction;
		yy -= ly * fraction;
	}

    glutPostRedisplay();
}