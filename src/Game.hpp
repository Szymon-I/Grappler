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
#define PLACEMENT_N 10
enum modes{
    EASY=0,
    MEDIUM,
    HARD
};
class Game{
private:
    static const glm::vec2 placements[PLACEMENT_N];
public:
Game();
~Game();
};
const glm::vec2 Game::placements[PLACEMENT_N] = {

};