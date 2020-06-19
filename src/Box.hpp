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
    static map<int, bool> falling_status;
    static int id_counter;
    ProgramHandler *box_program;
    bool grabbed = false;
    int id;
    bool falling = false;
    int level = 0;

public:
    Box();
    // init object with program
    Box(ProgramHandler *program)
    {
        id = id_counter++;
        this->box_program = program;
        glm::vec3 pos = program->get_translate();
        box_locations.insert(pair<int, glm::vec3>(id, pos));
        falling_status.insert(pair<int, bool>(id, false));
    };
    ~Box()
    {
        box_program->clean();
    };
    // display box according to grabbed status
    void display(glm::vec3 grappler_pos, glm::mat4x4 Matrix_proj, glm::mat4x4 Matrix_mv)
    {
        if (this->grabbed)
        {
            this->box_program->set_translate(grappler_pos + grappler_offset);
        }
        this->box_program->display(Matrix_proj, Matrix_mv, this->grabbed);
    }
    bool is_inside_field(glm::vec3 field_pos)
    {
        glm::vec3 pos = box_program->get_translate();
        float len = glm::length(glm::vec2(field_pos.x, field_pos.z) - glm::vec2(pos.x, pos.z));
        if (len < (2 * sqrt(2) * (box_size / 2)))
        {
            return true;
        }
        return false;
    }
    // take gravitation step on timer event
    bool update_gravitation()
    {
        glm::vec3 pos = box_program->get_translate();
        if (this->grabbed || pos.y <= 0 || !this->falling)
        {
            return false;
        }
        float max_level = -box_size;
        // check if anybox is underneath
        for (pair<int, glm::vec3> p : box_locations)
        {
            if (p.first == this->id)
            {
                continue;
            }
            // get length in x,z plane
            float len = glm::length(glm::vec2(p.second.x, p.second.z) - glm::vec2(pos.x, pos.z));
            if (len < (2 * sqrt(2) * (box_size / 2)) && !falling_status[p.first])
            {
                // update max level of stacked boxes in x/z coords
                max_level = max(max_level, p.second.y);
            }
        }
        // calculate new position
        float new_y = pos.y - GRAVITATION_PULL;
        // if new position is on ground
        if (new_y <= 0)
        {
            glm::vec3 new_pos = glm::vec3(pos.x, 0.0, pos.z);
            box_program->set_translate(new_pos);
            falling_status[this->id] = this->falling = false;
            box_locations[this->id] = new_pos;
        }
        // if new position is stacked on boxes
        else if (new_y <= (max_level + box_size))
        {
            glm::vec3 new_pos = glm::vec3(pos.x, (max_level + box_size), pos.z);
            box_program->set_translate(new_pos);
            falling_status[this->id] = this->falling = false;
            box_locations[this->id] = new_pos;
        }
        // if still falling
        else
        {
            glm::vec3 new_pos = glm::vec3(pos.x, new_y, pos.z);
            box_program->set_translate(new_pos);
            falling_status[this->id] = this->falling = true;
            box_locations[this->id] = new_pos;
        }
        // position is updated
        return true;
    }
    // get position of box
    glm::vec3 get_translate()
    {
        return this->box_program->get_translate();
    }
    // change grabbed status
    void change_grab(bool status)
    {
        this->grabbed = status;
    }
    // update location of box in map
    void update_location()
    {
        box_locations[this->id] = this->box_program->get_translate();
    }
    // get grabbed status
    bool is_grabbed()
    {
        return this->grabbed;
    }
    // set falling status
    void set_falling(bool status)
    {
        this->falling = status;
    }
    bool is_falling()
    {
        return this->falling;
    }
};
// offset of box grabbed by grappler
glm::vec3 Box::grappler_offset = glm::vec3(0.0, -4.5, 0.0);
// unique box size (without scaling)
float Box::box_size = 2.0;
// maps of box locations and status of freefall
// unique object id ->hash-> data
map<int, glm::vec3> Box::box_locations;
map<int, bool> Box::falling_status;
// counter for giving unique id on runtime
int Box::id_counter = 0;