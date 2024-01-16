#include "Camera.h"

// Constructor with vectors
Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    this->Position = position;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
    this->Target = target;
    this->isMooved = false;
    lastx = 200;
    lasty = 200;
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
    this->Target = target;
    this->isMooved = false;
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Target, glm::vec3(0, 1, 0));
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->MovementSpeed * deltaTime;
    glm::vec3 del = this->Target - this->Position;
    GLfloat curY = this->Position.y; // for saving Y coord const
    if (direction == FORWARD){
        this->Position += glm::normalize(del) * velocity;
        //this->Target += glm::normalize(del)* velocity;
    }
    if (direction == BACKWARD) {
        this->Position -= glm::normalize(del) * velocity;
        //this->Target -= glm::normalize(del) * velocity;
    }
    if (direction == LEFT) {
        this->Position -= glm::normalize(glm::cross(del, glm::vec3(0.f, 1.f, 0.f))) * velocity;
       // this->Target -= glm::normalize(glm::cross(del, glm::vec3(0.f, 1.f, 0.f))) * velocity;
    }
    if (direction == RIGHT) {
        this->Position += glm::normalize(glm::cross(del, glm::vec3(0.f, 1.f, 0.f))) * velocity;
       // this->Target += glm::normalize(glm::cross(del, glm::vec3(0.f, 1.f, 0.f))) * velocity;
    }
    this->Position.y = curY;
    this->isMooved = true;
    this->isDisplaced = true;
    sf::Listener::setPosition(Position.x, Position.y, Position.z);

    updateCameraVectors();
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(GLfloat x, GLfloat y, GLboolean constrainPitch = true)
{
    GLfloat xoffset = (x - lastx) * this->MouseSensitivity;
    GLfloat yoffset = (y - lasty) * this->MouseSensitivity;
    this->lastx = x;
    this->lasty = y;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.f)
            this->Pitch = 89.f;
        if (this->Pitch < -89.f)
            this->Pitch = -89.f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}
    // Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
    front.y = sin(glm::radians(Pitch));
    front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
    this->Target = this->Position - glm::normalize(front);
    this->isMooved = true;
    sf::Listener::setDirection(-front.x, -front.y, -front.z);
}