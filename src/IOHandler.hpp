#pragma once
#include <GL/freeglut.h>
#include <stdio.h>
#include "main.hpp"

#define ESC_KEY 27

extern Camera camera;

int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;

// --------------------------------------------------------------
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
// --------------------------------------------------------------
void Keyboard(unsigned char key, int x, int y)
{
	// keyboards events - not for camera
	switch (key)
	{
	case ESC_KEY:
		exit(0);
	}
	// camera events
	if (camera.get_mode() == THIRD_PERSON)
	{
		return;
	}
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

// --------------------------------------------------------------
void MouseButton(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON)
	{

		_mouse_buttonState = state;

		if (state == GLUT_DOWN)
		{
		}
	}
	// camera events
	if (camera.get_mode() == THIRD_PERSON)
	{
		return;
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

// --------------------------------------------------------------
void MouseMotion(int x, int y)
{
	camera.move_on_mouse_movement(x, y);
}
