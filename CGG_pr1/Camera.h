#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SFML/Audio.hpp"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat YAW = 0.0f;
const GLfloat PITCH = -90.0f;
const GLfloat SPEED = 3.1f;
const GLfloat SENSITIVITY = 0.2f;


// A camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera final
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Target;
    // Eular Angles
    GLfloat Yaw; //Рысканье по X
    GLfloat Pitch; //Тангаж по Y
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;

    bool isMooved;
    bool isDisplaced;
    // Constructor with vectors
    Camera(glm::vec3 position, glm::vec3 target, GLfloat yaw, GLfloat pitch) ;
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, glm::vec3 target, GLfloat yaw, GLfloat pitch);

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat x, GLfloat y, GLboolean constrainPitch);

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
    GLfloat lastx, lasty;
};