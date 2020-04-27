#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Light{
    private:
        glm::vec3 Light_Ambient;
        glm::vec3 Light_Diffuse;
        glm::vec3 Light_Position;
    public:
        void init(glm::vec3 ambient,glm::vec3 diffuse, glm::vec3 position){
            this->Light_Ambient=ambient;
            this->Light_Diffuse=diffuse;
            this->Light_Position=position;
        }
        glm::vec3 get_ambient(){
            return this->Light_Diffuse;
        }
        glm::vec3 get_diffuse(){
            return this->Light_Ambient;
        }
        glm::vec3 get_position(){
            return this->Light_Position;
        }
};