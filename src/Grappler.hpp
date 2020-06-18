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
#include "Box.hpp"

// paremetrs to filter noise from uC
#define Y_THRESHOLD 0.02
#define DATA_NORMALIZER 1000.0
#define Y_OFFSET 3.0
#define MOVEMENT_THRESHOLD 0.05
class Grappler
{
private:
    static float grab_range;
    // program for grappler object and actual position of it
    ProgramHandler program;
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 next_position = {0.0f, 0.0f, 0.0f};
    float sensitivity;
    float y_sensitivity = 0.0f;
    bool monitor_position = false;
    float prev_y;
    bool grabbed = false;
    int grabbed_id = 0;
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
        next_position.x = position.x + parse_data_float(pos_s[0]) * sensitivity;
        next_position.z = position.z + parse_data_float(pos_s[1]) * sensitivity;

        // y position is absolute
        float y_pos = parse_data_float(pos_s[2]);
        if (abs(position.y - y_pos) > Y_THRESHOLD)
        {
            next_position.y = parse_data_float(pos_s[2]) * y_sensitivity + Y_OFFSET;
        }
    }
    // print position of grappler
    void print_position()
    {
        printf("x: %f \t y: %f \t z: %f\n", position.x, position.y, position.z);
    }
    bool is_colliding(vector<ProgramHandler *> AllPrograms)
    {
        for (int i = 0; i < AllPrograms.size(); i++)
        {
            if (this->program.get_id() == AllPrograms[i]->get_id() || !AllPrograms[i]->get_collidable())
            {
                continue;
            }
            glm::vec3 obj_pos = AllPrograms[i]->get_translate();
            if ((float)glm::length(obj_pos - this->next_position) <= (this->program.get_collision_radius() + AllPrograms[i]->get_collision_radius()))
            {
                return true;
            }
        }
        return false;
    }

public:
    // initialize grappler program with object program and settings
    void init(ProgramHandler program, float sensitivity = 1, float y_sensitivity = 5)
    {
        this->program = program;
        this->position = program.get_translate();
        this->next_position = this->position;
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
        this->next_position = this->position;
        program.set_translate(position);
    }
    // move grappler
    void move_grappler(std::string data, vector<ProgramHandler *> AllPrograms)
    {
        if (monitor_position)
        {
            print_position();
        }
        parse_position(data);
        if ((float)glm::length(this->position - this->next_position) < MOVEMENT_THRESHOLD)
        {
            return;
        }
        if (!is_colliding(AllPrograms))
        {

            program.set_translate(this->next_position);
            this->position = this->next_position;
        }
        else
        {
            printf("colliding\n");
        }
        //program.set_translate(position);
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
    void grab_object(vector<Box *> Boxes)
    {
        if (this->grabbed)
        {
            Boxes[grabbed_id]->change_grab(false);
            this->grabbed=false;
            return;
        }
        int min_index = 0;
        float min_value = glm::length(Boxes[0]->get_translate() - this->get_position());
        // get index of closest box
        for (int i = 0; i < Boxes.size(); i++)
        {
            float len = glm::length(Boxes[i]->get_translate() - this->get_position());
            if (min_value > len)
            {
                min_index = i;
                min_value = len;
            }
        }
        // check if closest box is close enough to grab
        if (grab_range >= min_value)
        {
            this->grabbed = true;
            this->grabbed_id = min_index;
            Boxes[min_index]->change_grab(true);
        }
    }
};

float Grappler::grab_range = 4.5;