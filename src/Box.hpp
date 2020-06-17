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
#define GRAVITATION_PULL 0.1
class Box
{
private:
    static glm::vec3 grappler_offset;
    ProgramHandler *box_program;
    bool grabbed = false;

public:
    Box(/* args */);
    Box(ProgramHandler *program)
    {
        this->box_program = program;
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
        bool updated = false;
        glm::vec3 pos = box_program->get_translate();
        if (pos.y > 0)
        {
            updated = true;
            float new_y = pos.y - GRAVITATION_PULL;
            if (new_y < 0)
            {
                box_program->set_translate(glm::vec3(pos.x, 0.0f, pos.z));
            }
            else
            {
                box_program->set_translate(glm::vec3(pos.x, new_y, pos.z));
            }
        }
        return updated;
    }
    glm::vec3 get_translate()
    {
        return this->box_program->get_translate();
    }

    void change_grab(bool status){
        this->grabbed = status;
    }
};
glm::vec3 Box::grappler_offset = glm::vec3(0.0, -4.0, 0.0);