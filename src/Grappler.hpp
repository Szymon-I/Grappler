#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <iostream>
#include "ProgramHandler.hpp"

// paremetrs to filter noise from uC
#define Y_THRESHOLD 0.02
#define DATA_NORMALIZER 1000.0
#define Y_OFFSET 3.0
class Grappler
{
private:
    // program for grappler object and actual position of it
    ProgramHandler program;
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    float sensitivity;
    float y_sensitivity = 0.0f;
    bool monitor_position = false;
    float prev_y;

    // parse data from string to float and normalize
    float parse_data_float(std::string data)
    {
        return atof(data.c_str()) / DATA_NORMALIZER;
    }
    // get actual data from serial and parse it to grappler position
    // serial input is in format <x_acc>/<y_acc>/<potentiometer>
    void parse_position(std::string s)
    {
        int i = 0;
        std::string pos_s[3];
        std::string delimiter = "/";
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos)
        {
            token = s.substr(0, pos);
            pos_s[i++] = token;
            s.erase(0, pos + delimiter.length());
        }
        pos_s[i] = s;
        position.x += parse_data_float(pos_s[0]) * sensitivity;
        position.z += parse_data_float(pos_s[1]) * sensitivity;

        // y position is absolute
        float y_pos = parse_data_float(pos_s[2]);
        if (abs(prev_y - y_pos) > Y_THRESHOLD)
        {
            position.y = parse_data_float(pos_s[2]) * y_sensitivity + Y_OFFSET;
            prev_y = y_pos;
        }
    }
    // print position of grappler
    void print_position()
    {
        printf("x: %f \t y: %f \t z: %f\n", position.x, position.y, position.z);
    }

public:
    // initialize grappler program with object program and settings
    void init(ProgramHandler program, float sensitivity = 1, float y_sensitivity = 5)
    {
        this->program = program;
        this->sensitivity = sensitivity;
        this->y_sensitivity = y_sensitivity;
    }
    // turn on/off serial monitor
    void set_monitor(bool x)
    {
        this->monitor_position = x;
    }
    // get actual position of grappler
    glm::vec3 get_position()
    {
        return this->position;
    }
    void set_position(glm::vec3 position)
    {
        this->position = position;
        program.set_translate(position);
    }
    // move grappler
    void move_grappler(std::string data)
    {
        parse_position(data);
        if (monitor_position)
        {
            print_position();
        }
        program.set_translate(position);
    }
    // display grappler
    void display_grappler(glm::mat4x4 Matrix_proj, glm::mat4x4 Matrix_mv, Camera camera)
    {
        if (camera.get_mode() == THIRD_PERSON)
        {
            program.set_translate(camera.GetCameraPos() + camera.get_offsets());
        }
        program.display(Matrix_proj, Matrix_mv);
    }
};