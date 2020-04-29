#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define EDGE_CORRECTION 40

#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080

#define CAMERA_MOVE_LEFT_KEY 'a'
#define CAMERA_MOVE_RIGHT_KEY 'd'
#define CAMERA_MOVE_FORWARD_KEY 'w'
#define CAMERA_MOVE_BACK_KEY 's'

class Camera
{
private:
    // ------------ MOUSE VARIABLES ----------------------//

    // angle of rotation for the camera direction
    GLfloat angle = 0.0;
    // actual vector representing the camera’s direction
    GLfloat lx = 1.0f, lz = -1.0f, ly = 0.0f;
    // XZ position of the camera
    GLfloat xx = 0.0f, zz = -1.0f, yy = 1.0f;
    // mouse variables
    GLfloat fraction = 0.2f;
    GLfloat mouseOnEdgeSpeed = fraction / 10;
    int oldMouseX = DEFAULT_WINDOW_WIDTH / 2, oldMouseY = DEFAULT_WINDOW_HEIGHT / 2;

    int windowHeight = DEFAULT_WINDOW_HEIGHT, windowWidth = DEFAULT_WINDOW_WIDTH;
    GLfloat _scene_rotate_x;
    GLfloat _scene_rotate_y;
    GLfloat _scene_translate_x;
    GLfloat _scene_translate_y;
    GLfloat _scene_translate_z;

public:
    Camera()
    {
        this->_scene_rotate_x = 0.0f;
        this->_scene_rotate_y = 0.0f;
        this->_scene_translate_x = 0.0f;
        this->_scene_translate_y = 0.0f;
        this->_scene_translate_z = 0.0f;
    }
    Camera(GLfloat translate_x, GLfloat translate_y, GLfloat translate_z)
    {
        this->_scene_translate_x = translate_x;
        this->_scene_translate_y = translate_y;
        this->_scene_translate_z = translate_z;
    }
    Camera(GLfloat rotate_x, GLfloat rotate_y, GLfloat translate_x, GLfloat translate_y, GLfloat translate_z)
    {
        this->_scene_rotate_x = rotate_x;
        this->_scene_rotate_y = rotate_y;
        this->_scene_translate_x = translate_x;
        this->_scene_translate_y = translate_y;
        this->_scene_translate_z = translate_z;
    }
    void set_window_dimenstions(int width, int height)
    {
        windowWidth = width;
        windowHeight = height;
    }
    void set_camera_sensitivity(GLfloat sensitivity)
    {
        this->fraction = sensitivity;
    }
    void move_left()
    {
        xx -= cos(angle);
        zz -= sin(angle);
    }
    void move_right()
    {
        xx += cos(angle);
        zz += sin(angle);
    }
    void move_forward()
    {
        xx += lx * fraction;
        zz += lz * fraction;
        yy += ly * fraction;
    }
    void move_back()
    {
        xx -= lx * fraction;
        zz -= lz * fraction;
        yy -= ly * fraction;
    }
    void move_on_mouse_movement(int x, int y)
    {
        angle += (x - oldMouseX) / 100.0;
        ly -= (y - oldMouseY) / 100.0;

        lx = sin(angle);
        lz = -cos(angle);

        oldMouseX = x;
        oldMouseY = y;
    }
    void mouse_edge()
    {
        if (oldMouseX > windowWidth - EDGE_CORRECTION)
        {
            angle += mouseOnEdgeSpeed;
            lx = sin(angle);
            lz = -cos(angle);
        }
        else if (oldMouseX < EDGE_CORRECTION)
        {
            angle -= mouseOnEdgeSpeed;
            lx = sin(angle);
            lz = -cos(angle);
        }
    }
    glm::mat4x4 apply_camera(glm::mat4x4 Matrix_mv)
    {
        Matrix_mv = glm::lookAt(glm::vec3(xx, yy, zz),
                                glm::vec3(xx + lx, yy + ly, zz + lz),
                                glm::vec3(0.0f, 1.0f, 0.0f));
        Matrix_mv = glm::translate(Matrix_mv, glm::vec3(_scene_translate_x, _scene_translate_y, _scene_translate_z));
        Matrix_mv = glm::rotate(Matrix_mv, _scene_translate_x + _scene_rotate_x, glm::vec3(1.0f, 0.0f, 0.0f));
        Matrix_mv = glm::rotate(Matrix_mv, _scene_translate_y + _scene_rotate_y, glm::vec3(0.0f, 1.0f, 0.0f));
        return Matrix_mv;
    }
};