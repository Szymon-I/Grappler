#pragma once
#include <GL/freeglut.h>
#include <stdio.h>
#include "main.hpp"

#define ESC_KEY 27
#define GRAB_KEY 'g'
#define SPACEBAR 32
#define MOVE_UP '+'
#define MOVE_DOWN '-'
#define VERTICAL_MOEVEMENT (DATA_NORMALIZER / 20)
extern Camera camera;
extern vector<Box *> Boxes;
extern bool serial_attached;
extern float grappler_y;

int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;

// handler for special keys on keyboard
void SpecialKeys(int key, int x, int y)
{
	string y_val = "0.0";
	if (!serial_attached)
	{
		y_val = std::to_string(grappler_y);
	}
	switch (key)
	{
	case GLUT_KEY_LEFT:
		grappler.move_grappler(std::to_string(-DATA_NORMALIZER) + "/0.0/" + y_val, AllPrograms);
		break;

	case GLUT_KEY_RIGHT:
		grappler.move_grappler(std::to_string(DATA_NORMALIZER) + "/0.0/" + y_val, AllPrograms);
		break;

	case GLUT_KEY_UP:
		grappler.move_grappler("0.0/" + std::to_string(-DATA_NORMALIZER) + "/" + y_val, AllPrograms);
		break;

	case GLUT_KEY_DOWN:
		grappler.move_grappler("0.0/" + std::to_string(DATA_NORMALIZER) + "/" + y_val, AllPrograms);
		break;
	}
}
//handler for keyboard events
void Keyboard(unsigned char key, int x, int y)
{
	// keyboards events - not for camera
	switch (key)
	{
	case SPACEBAR:
		grappler.grab_object(Boxes);
		break;
	case ESC_KEY:
		exit(0);
		break;
	}
	if (!serial_attached)
	{
		switch (key)
		{
		case MOVE_UP:
			grappler_y += VERTICAL_MOEVEMENT;
			grappler.move_grappler("0.0/0.0/" + std::to_string(grappler_y), AllPrograms);
			break;
		case MOVE_DOWN:
			grappler_y -= VERTICAL_MOEVEMENT;
			grappler.move_grappler("0.0/0.0/" + std::to_string(grappler_y), AllPrograms);
			break;
		}
	}

	// camera events
	// camera position is relative to grappler
	if (camera.get_mode() == THIRD_PERSON)
	{
		// simulate serial input for program with 'w,a,s,d' (move grappler)
		switch (key)
		{
		case CAMERA_MOVE_RIGHT_KEY:
			grappler.move_grappler(std::to_string(DATA_NORMALIZER) + "/0.0/0.0", AllPrograms);
			break;
		case CAMERA_MOVE_LEFT_KEY:
			grappler.move_grappler(std::to_string(-DATA_NORMALIZER) + "/0.0/0.0", AllPrograms);
			break;
		case CAMERA_MOVE_BACK_KEY:
			grappler.move_grappler("0.0/" + std::to_string(DATA_NORMALIZER) + "/0.0", AllPrograms);
			break;
		case CAMERA_MOVE_FORWARD_KEY:
			grappler.move_grappler("0.0/" + std::to_string(-DATA_NORMALIZER) + "/0.0", AllPrograms);
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
