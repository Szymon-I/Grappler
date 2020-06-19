#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Text.hpp"
#include <stdlib.h>
#include <iostream>
#include "ProgramHandler.hpp"
#define PLACEMENT_N 10
enum modes
{
    EASY = 1,
    MEDIUM,
    HARD
};
class Game
{
private:
    static const glm::vec2 placements[PLACEMENT_N];
    bool placement_used[PLACEMENT_N] = {false};
    int placement_left = PLACEMENT_N;
    int points = -1;
    int upper_bound;
    int mode;
    bool finished = false;
    glm::vec2 active_field;
    ProgramHandler *game_box;
    Text score;
    Text left;
    Text finish_text;
    void display_score()
    {
        // display score
        char str[10];
        snprintf(str, sizeof(str), "%d", this->points);
        std::string s(str);
        score.RenderText("Score: " + s, 10, glutGet(GLUT_WINDOW_HEIGHT) - 100, 0.8f, glm::vec3(1.0, 0.0f, 0.0f));
        // display how much left
        snprintf(str, sizeof(str), "%d", this->placement_left);
        std::string s2(str);
        score.RenderText("Left: " + s2, 10, glutGet(GLUT_WINDOW_HEIGHT) - 130, 0.8f, glm::vec3(1.0, 0.0f, 0.0f));
    }
    void display_finished()
    {
        finish_text.RenderText("Game finished", 10, glutGet(GLUT_WINDOW_HEIGHT) - 100, 0.8f, glm::vec3(1.0, 0.0f, 0.0f));
    }

public:
    Game(modes m, ProgramHandler *game_box)
    {
        this->mode = m;
        this->game_box = game_box;
        // first step
        step();
    }
    ~Game()
    {
        delete this->game_box;
    }
    void reshape_text()
    {
        score.InitText((char *)"libs/arial.ttf", 36);
        left.InitText((char *)"libs/arial.ttf", 36);
        finish_text.InitText((char *)"libs/arial.ttf", 36);
    }
    void step()
    {
        if (placement_left == 0)
        {
            this->finished = true;
            return;
        }
        // get next field
        int r = std::rand() % placement_left;
        int i = 0;
        int select = 0;
        while (select < r)
        {
            if (!placement_used[i])
            {
                select++;
            }
            i++;
        }
        active_field = placements[select];
        placement_left--;
        placement_used[select] = true;
        this->points++;
    }
    void display(glm::mat4x4 Matrix_proj, glm::mat4x4 Matrix_mv)
    {
        if (!finished)
        {

            display_score();
            game_box->set_translate(glm::vec3(active_field.x, 0.0, active_field.y));
            game_box->display(Matrix_proj, Matrix_mv);
        }
        else
        {
            display_finished();
        }
    }
    glm::vec3 get_active_field()
    {
        return glm::vec3(this->active_field.x, 0.0, this->active_field.y);
    }
};
const glm::vec2 Game::placements[PLACEMENT_N] = {
    glm::vec2(0.0, 0.0),
    glm::vec2(2.0, -3.0),
    glm::vec2(4.0, -4.0),
    glm::vec2(6.0, 6.0),
    glm::vec2(18.0, 5.0),
    glm::vec2(10.0, -16.0),
    glm::vec2(12.0, 10.0),
    glm::vec2(13.0, -2.0),
    glm::vec2(16.0, -16.0),
    glm::vec2(-18.0, 5.0)};