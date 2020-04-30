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

        std::vector < glm::vec3 > material;
        material.push_back(ambient);
        material.push_back(diffuse);
        material.push_back(specular);
        material.push_back(shine);

        return material;
    }

    static std::vector < glm::vec3 > BlackRubber(void)
    {
        glm::vec3 ambient = glm::vec3(0.02f, 0.02f, 0.02f);
        glm::vec3 diffuse = glm::vec3(0.01f, 0.01f, 0.01f);
        glm::vec3 specular = glm::vec3(0.4f, 0.4f, 0.4f);
        glm::vec3 shine = glm::vec3(10.0f, 10.0f, 10.0f);

        std::vector < glm::vec3 > material;
        material.push_back(ambient);
        material.push_back(diffuse);
        material.push_back(specular);
        material.push_back(shine);

        return material;
    }

    static std::vector < glm::vec3 > Emerald(void)
    {
        glm::vec3 ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
        glm::vec3 diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
        glm::vec3 specular = glm::vec3(0.633f, 0.727811f, 0.633f);
        glm::vec3 shine = glm::vec3(76.8f, 76.8f, 76.8f);

        std::vector < glm::vec3 > material;
        material.push_back(ambient);
        material.push_back(diffuse);
        material.push_back(specular);
        material.push_back(shine);

        return material;
    }

    static std::vector < glm::vec3 > Tin(void)
    {
        glm::vec3 ambient = glm::vec3(0.105882f, 0.058824f, 0.113725f);
        glm::vec3 diffuse = glm::vec3(0.427451f, 0.470588f, 0.541176f);
        glm::vec3 specular = glm::vec3(0.333333f, 0.333333f, 0.521569f);
        glm::vec3 shine = glm::vec3(9.84615f, 9.84615f, 9.84615f);

        std::vector < glm::vec3 > material;
        material.push_back(ambient);
        material.push_back(diffuse);
        material.push_back(specular);
        material.push_back(shine);

        return material;
    }

        static std::vector < glm::vec3 > WhiteRubber(void)
    {
        glm::vec3 ambient = glm::vec3(0.05f,0.05f,0.05f);
        glm::vec3 diffuse = glm::vec3(0.5f,0.5f,0.5f);
        glm::vec3 specular = glm::vec3(0.7f,0.7f,0.7f);
        glm::vec3 shine = glm::vec3(10.0f, 10.0f, 10.0f);

        std::vector < glm::vec3 > material;
        material.push_back(ambient);
        material.push_back(diffuse);
        material.push_back(specular);
        material.push_back(shine);

        return material;
    }

};