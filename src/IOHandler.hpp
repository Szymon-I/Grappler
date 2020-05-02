#pragma once
#include <GL/freeglut.h>
#include <stdio.h>
#include "main.hpp"

#define ESC_KEY 27

extern Camera camera;

int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;

// handler for special keys on keyboard
void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_RIGHT:
		break;

	case GLUT_KEY_UP:
		break;

	case GLUT_KEY_DOWN:
		break;
	}
}
//handler for keyboard events
void Keyboard(unsigned char key, int x, int y)
{
	// keyboards events - not for camera
	switch (key)
	{
	case ESC_KEY:
		exit(0);
	}

	// camera events
	// camera position is relative to grappler
	if (camera.get_mode() == THIRD_PERSON)
	{
		// simulate serial input for program with 'w,a,s,d' (move grappler)
		switch (key)
		{
		case CAMERA_MOVE_RIGHT_KEY:
			grappler.move_grappler(std::to_string(DATA_NORMALIZER) + "/0.0/0.0");
			break;
		case CAMERA_MOVE_LEFT_KEY:
			grappler.move_grappler(std::to_string(-DATA_NORMALIZER) + "/0.0/0.0");
			break;
		case CAMERA_MOVE_BACK_KEY:
			grappler.move_grappler("0.0/" + std::to_string(DATA_NORMALIZER) + "/0.0");
			break;
		case CAMERA_MOVE_FORWARD_KEY:
			grappler.move_grappler("0.0/" + std::to_string(-DATA_NORMALIZER) + "/0.0");
			break;
		}
	}
	// camera position is independent to grappler
	else if (camera.get_mode() == FREE_CAMERA)
	{
		switch (key)
		{
		case CAMERA_MOVE_LEFT_KEY:
			camera.move_left();
			break;
		case CAMERA_MOVE_RIGHT_KEY:
			camera.move_right();
			break;
		case CAMERA_MOVE_FORWARD_KEY:
			camera.move_forward();
			break;
		case CAMERA_MOVE_BACK_KEY:
			camera.move_back();
			break;
		}
	}
	glutPostRedisplay();
}

// handle mouse buttons events
void MouseButton(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON)
	{

		_mouse_buttonState = state;
		if (state == GLUT_DOWN)
		{
		}
	}
	// scroll up
	if (button == 3)
	{
		camera.move_forward();
	}
	// scroll down
	if (button == 4)
	{
		camera.move_back();
	}
}

// look around with mouse motion
void MouseMotion(int x, int y)
{
	camera.move_on_mouse_movement(x, y);
}
