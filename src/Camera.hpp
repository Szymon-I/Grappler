#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Grappler.hpp"

// paramater for correcting camera while mouse is outside window
#define EDGE_CORRECTION 40

// default window parameters
#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080

// key bindings for moving camera
#define CAMERA_MOVE_LEFT_KEY 'a'
#define CAMERA_MOVE_RIGHT_KEY 'd'
#define CAMERA_MOVE_FORWARD_KEY 'w'
#define CAMERA_MOVE_BACK_KEY 's'

// available camera modes
enum CAMERA_MODES
{
    FREE_CAMERA = 0,
    THIRD_PERSON
};

class Camera
{
private:
    // angle of rotation for the camera direction
    GLfloat angle = 0.0;
    // actual vector representing the camera’s direction
    GLfloat lx = 1.0f, lz = -1.0f, ly = 0.0f;
    // XZ position of the camera
    GLfloat xx = 0.0f, zz = -1.0f, yy = 1.0f;

    // mouse sensitivity
    GLfloat fraction = 0.1f;
    GLfloat mouseOnEdgeSpeed = fraction / 10;

    // window coordinates
    int oldMouseX = DEFAULT_WINDOW_WIDTH / 2, oldMouseY = DEFAULT_WINDOW_HEIGHT / 2;
    int windowHeight = DEFAULT_WINDOW_HEIGHT, windowWidth = DEFAULT_WINDOW_WIDTH;

    // parameters for scene
    GLfloat _scene_rotate_x;
    GLfloat _scene_rotate_y;
    GLfloat _scene_translate_x;
    GLfloat _scene_translate_y;
    GLfloat _scene_translate_z;

    // actual scene Matrix_mv
    glm::mat4x4 Matrix_mv;

    // actual camera mode
    CAMERA_MODES mode = FREE_CAMERA;

    // offsets to move camera from grappler object in THIRD_PERSON mode
    glm::vec3 THIRD_PERSON_OFFSETS = glm::vec3(0.0f, -2.0f, 2.0f);

public:
    // initialize Camera with or without offests
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
    // get camera mode
    CAMERA_MODES get_mode()
    {
        return this->mode;
    }
    // set camera mode
    void set_mode(CAMERA_MODES mode, glm::vec3 pos)
    {
        this->mode = mode;
        if (mode == THIRD_PERSON)
        {
            set_look(pos);
        }
    }
    // get camera offsets
    glm::vec3 get_offsets()
    {
        return this->THIRD_PERSON_OFFSETS;
    }
    // set look for camera
    void set_look(glm::vec3 pos)
    {
        xx = pos.x;
        zz = pos.y;
        yy = pos.z;
    }
    //set translate for camera
    void set_translate(glm::vec3 pos)
    {
        _scene_translate_x = pos.x;
        _scene_translate_y = pos.y;
        _scene_translate_z = pos.z;
    }
    // set window width and height (after reshape)
    void set_window_dimenstions(int width, int height)
    {
        windowWidth = width;
        windowHeight = height;
    }
    // set camera sensitivity
    void set_camera_sensitivity(GLfloat sensitivity)
    {
        this->fraction = sensitivity;
    }
    // move camera left (strafing)
    void move_left()
    {
        xx -= cos(angle) * fraction;
        zz -= sin(angle) * fraction;
    }
    // move camera right (strafing)
    void move_right()
    {
        xx += cos(angle) * fraction;
        zz += sin(angle) * fraction;
    }
    // move camera forward (zoom in)
    void move_forward()
    {
        xx += lx * fraction;
        zz += lz * fraction;
        yy += ly * fraction;
    }
    // move camera forward (zoom out)
    void move_back()
    {
        xx -= lx * fraction;
        zz -= lz * fraction;
        yy -= ly * fraction;
    }
    // move camera with mouse (look around)
    void move_on_mouse_movement(int x, int y)
    {
        angle += (x - oldMouseX) / 100.0;
        ly -= (y - oldMouseY) / 100.0;

        lx = sin(angle);
        lz = -cos(angle);

        oldMouseX = x;
        oldMouseY = y;
    }
    // move camera if mouse is outside program window
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
    // apply camera for actual scene with given camera mode (modify Matrix_mv)
    glm::mat4x4 apply_camera(glm::mat4x4 Matrix_mv, glm::vec3 grappler_pos)
    {
        switch (mode)
        {
        case FREE_CAMERA:
            Matrix_mv = glm::lookAt(glm::vec3(xx, yy, zz),
                                    glm::vec3(xx + lx, yy + ly, zz + lz),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
            Matrix_mv = glm::translate(Matrix_mv, glm::vec3(_scene_translate_x, _scene_translate_y, _scene_translate_z));
            Matrix_mv = glm::rotate(Matrix_mv, _scene_translate_x + _scene_rotate_x, glm::vec3(1.0f, 0.0f, 0.0f));
            Matrix_mv = glm::rotate(Matrix_mv, _scene_translate_y + _scene_rotate_y, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case THIRD_PERSON:
            Matrix_mv = glm::lookAt(glm::vec3(xx, yy, zz),
                                    glm::vec3(xx + lx, yy + ly, zz + lz),
                                    glm::vec3(0.0f, 1.0f, 0.0f));

            Matrix_mv = glm::translate(Matrix_mv, grappler_pos + THIRD_PERSON_OFFSETS);
            Matrix_mv = glm::rotate(Matrix_mv, _scene_translate_x + _scene_rotate_x, glm::vec3(1.0f, 0.0f, 0.0f));
            Matrix_mv = glm::rotate(Matrix_mv, _scene_translate_y + _scene_rotate_y, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        default:
            break;
        }

        this->Matrix_mv = Matrix_mv;
        return Matrix_mv;
    }
    // get actual camera position
    glm::vec3 GetCameraPos(void)
    {
        glm::mat4 modelViewT = transpose(this->Matrix_mv);

        glm::vec3 n1(modelViewT[0]);
        glm::vec3 n2(modelViewT[1]);
        glm::vec3 n3(modelViewT[2]);

        float d1(modelViewT[0].w);
        float d2(modelViewT[1].w);
        float d3(modelViewT[2].w);

        glm::vec3 n2n3 = cross(n2, n3);
        glm::vec3 n3n1 = cross(n3, n1);
        glm::vec3 n1n2 = cross(n1, n2);

        glm::vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
        float denom = dot(n1, n2n3);

        return top / -denom;
    }
};