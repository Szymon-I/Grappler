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
#define GRAVITATION_PULL 0.02
class Box
{
private:
    ProgramHandler *box_program;

public:
    Box(/* args */);
    Box(ProgramHandler *program)
    {
        this->box_program = program;
    };
    ~Box(){
        box_program->clean();
    };
    bool update_gravitation()
    {
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
};
