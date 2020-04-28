#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "ProgramHandler.hpp"

class Grappler
{

private:
    ProgramHandler program;
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    float sensitivity;
    bool monitor_position;
    //serial input is in format <x_acc>/<y_acc>/<potentiometer>
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
        position.x += atof(pos_s[0].c_str()) * sensitivity;
        position.y += atof(pos_s[1].c_str()) * sensitivity;
        position.z += atof(pos_s[2].c_str()) * sensitivity;
    }
    void print_position()
    {
        printf("x: %f \t y: %f \t z: %f\n", position.x, position.y, position.z);
    }

public:
    void init(ProgramHandler program, float sensitivity = 1, bool monitor_position = true)
    {
        this->program = program;
        this->sensitivity = sensitivity;
        this->monitor_position = monitor_position;
    }
    void display_grappler(glm::mat4x4 Matrix_proj, glm::mat4x4 Matrix_mv)
    {
        program.display(Matrix_proj, Matrix_mv);
    }
    void move_grappler(std::string data)
    {
        parse_position(data);
        if (monitor_position)
        {
            print_position();
        }
        program.set_translate(position);
        glutPostRedisplay();
    }
};