#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// max boundary for movement in x axis
#define MAX_X 10.0
#define MIN_X -10.0
class Light{
private:
    // light parameters
    std::vector < glm::vec3 > Light_Ambient;
    std::vector < glm::vec3 > Light_Diffuse;
    std::vector < glm::vec3 > Light_Position;

    // light movement speed
    float light_movement = 0.1;

public:
        // initialize Global Light object with all lights parameters as lists
        void init(std::vector < glm::vec3 > ambient, std::vector < glm::vec3 > diffuse, std::vector < glm::vec3 > position){
            this->Light_Ambient=ambient;
            this->Light_Diffuse=diffuse;
            this->Light_Position=position;
        }
        // get ambient parameters
        std::vector < glm::vec3 > get_ambient(){
            return this->Light_Ambient;
        }
        // get diffuse parameters
        std::vector < glm::vec3 > get_diffuse(){
            return this->Light_Diffuse;
        }
        // get position parameters
        std::vector < glm::vec3 > get_position(){
            return this->Light_Position;
        }
        // move light
        void move_light(int index)
        {
            if (Light_Position[index].x > MAX_X || Light_Position[index].x < MIN_X)
			    light_movement = -light_movement;
		    Light_Position[index].x += light_movement;
        }
};