#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Light{
private:

    std::vector < glm::vec3 > Light_Ambient;
    std::vector < glm::vec3 > Light_Diffuse;
    std::vector < glm::vec3 > Light_Position;

    float light_movement = 0.1;

public:

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

        void move_light(int index)
        {
            if (Light_Position[index].z > 10.0 || Light_Position[index].z < -10.0)
			    light_movement = -light_movement;
		    Light_Position[index].z += light_movement;
        }
};