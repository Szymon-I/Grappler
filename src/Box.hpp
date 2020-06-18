#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <map> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <iostream>
#include "ProgramHandler.hpp"
#define GRAVITATION_PULL 0.1
class Box
{
private:
    static glm::vec3 grappler_offset;
    static float box_size;
    static map<int, glm::vec3> box_locations;
    static int id_counter;
    ProgramHandler *box_program;
    bool grabbed = false;
    int id;

public:
    Box(/* args */);
    Box(ProgramHandler *program)
    {
        id = id_counter++;
        this->box_program = program;
        box_locations.insert(pair<int, glm::vec3> (id,program->get_translate()));
    };
    ~Box()
    {
        box_program->clean();
    };
    void display(glm::vec3 grappler_pos, glm::mat4x4 Matrix_proj, glm::mat4x4 Matrix_mv)
    {
        if (this->grabbed)
        {
            this->box_program->set_translate(grappler_pos + grappler_offset);
        }
        this->box_program->display(Matrix_proj, Matrix_mv, this->grabbed);
    }
    bool update_gravitation()
    {
        if (this->grabbed)
        {
            return false;
        }

        glm::vec3 pos = box_program->get_translate();
        if (pos.y <= 0)
        {
            return false;
        }
        float box_underneath = false;
        // check if anybox is underneath
        for (pair<int, glm::vec3> p : box_locations)
        {
            if (p.first == this->id)
            {
                continue;
            }
            // get length in x,z plane
            float len = glm::length(glm::vec2(p.second.x, p.second.z) - glm::vec2(pos.x, pos.z));
            if (len < (2 * sqrt(2) * box_size))
            {
                box_underneath = true;
            }
        }

        float new_y = pos.y - GRAVITATION_PULL;
        if (new_y < 0 || ((new_y <= box_size) && box_underneath))
        {
            box_program->set_translate(glm::vec3(pos.x, box_underneath ? box_size : 0.0, pos.z));
        }
        else
        {
            box_program->set_translate(glm::vec3(pos.x, new_y, pos.z));
        }

        return true;
    }
    glm::vec3 get_translate()
    {
        return this->box_program->get_translate();
    }

    void change_grab(bool status)
    {
        this->grabbed = status;
    }
    void update_location(){
        box_locations[this->id] = this->box_program->get_translate();
    }
};
glm::vec3 Box::grappler_offset = glm::vec3(0.0, -4.5, 0.0);
float Box::box_size = 2.0;
map<int, glm::vec3> Box::box_locations;
int Box::id_counter = 0;