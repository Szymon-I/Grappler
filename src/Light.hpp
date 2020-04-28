#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Light{
    public:
        std::vector < glm::vec3 > Light_Ambient;
        std::vector < glm::vec3 > Light_Diffuse;
        std::vector < glm::vec3 > Light_Position;

        void init(std::vector < glm::vec3 > ambient, std::vector < glm::vec3 > diffuse, std::vector < glm::vec3 > position){
            this->Light_Ambient=ambient;
            this->Light_Diffuse=diffuse;
            this->Light_Position=position;
        }

        std::vector < glm::vec3 > get_ambient(){
            return this->Light_Ambient;
        }

        std::vector < glm::vec3 > get_diffuse(){
            return this->Light_Diffuse;
        }

        std::vector < glm::vec3 > get_position(){
            return this->Light_Position;
        }
};