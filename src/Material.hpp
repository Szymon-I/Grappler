#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Material{
public:

    static std::vector < glm::vec3 > Brass(void)
    {
        glm::vec3 ambient = glm::vec3(0.329412f, 0.223529f, 0.027451f);
        glm::vec3 diffuse = glm::vec3(0.780392f, 0.568627f, 0.113725f);
        glm::vec3 specular = glm::vec3(0.992157f, 0.941176f, 0.807843f);
        glm::vec3 shine = glm::vec3(27.8974f, 27.8974f, 27.8974f);

        std::vector < glm::vec3 > brass;
        brass.push_back(ambient);
        brass.push_back(diffuse);
        brass.push_back(specular);
        brass.push_back(shine);

        return brass;
    }


};