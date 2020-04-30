#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Material
{
public:
    const static std::vector<glm::vec3> Brass;
    const static std::vector<glm::vec3> BlackRubber;
    const static std::vector<glm::vec3> Emerald;
    const static std::vector<glm::vec3> Tin;
    const static std::vector<glm::vec3> WhiteRubber;
};

const std::vector<glm::vec3> Material::Brass{glm::vec3(0.329412f, 0.223529f, 0.027451f),
                                             glm::vec3(0.780392f, 0.568627f, 0.113725f),
                                             glm::vec3(0.992157f, 0.941176f, 0.807843f),
                                             glm::vec3(27.8974f, 27.8974f, 27.8974f)};

const std::vector<glm::vec3> Material::BlackRubber{glm::vec3(0.02f, 0.02f, 0.02f),
                                                   glm::vec3(0.01f, 0.01f, 0.01f),
                                                   glm::vec3(0.4f, 0.4f, 0.4f),
                                                   glm::vec3(10.0f, 10.0f, 10.0f)};

const std::vector<glm::vec3> Material::Emerald{glm::vec3(0.0215f, 0.1745f, 0.0215f),
                                               glm::vec3(0.07568f, 0.61424f, 0.07568f),
                                               glm::vec3(0.633f, 0.727811f, 0.633f),
                                               glm::vec3(76.8f, 76.8f, 76.8f)};

const std::vector<glm::vec3> Material::Tin{glm::vec3(0.105882f, 0.058824f, 0.113725f),
                                           glm::vec3(0.427451f, 0.470588f, 0.541176f),
                                           glm::vec3(0.333333f, 0.333333f, 0.521569f),
                                           glm::vec3(9.84615f, 9.84615f, 9.84615f)};

const std::vector<glm::vec3> Material::WhiteRubber{glm::vec3(0.05f, 0.05f, 0.05f),
                                                   glm::vec3(0.5f, 0.5f, 0.5f),
                                                   glm::vec3(0.7f, 0.7f, 0.7f),
                                                   glm::vec3(10.0f, 10.0f, 10.0f)};