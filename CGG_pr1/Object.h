#pragma once
#include "Model.h"
#include "tasks.h"
class Object : public Model
{
protected:
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > textures;
    std::vector< glm::vec3 > normals;
    std::vector< glm::vec3 > colors;
    std::vector< GLuint >    indices;

    glm::vec3 position = glm::vec3(0);
    GLfloat const speed = 0.01f;
    GLfloat degree = 0;
    GLfloat radius = 5.5;

    void _LoadAll();
public:
    Object(const char* pth, glm::vec3 Position = glm::vec3(0));
    Object(glm::vec3 Position = glm::vec3(0));
    int Setter(
        std::vector< glm::vec3 > vertices,
        std::vector< GLuint >    indices = {},
        std::vector< glm::vec3 > colors = {},
        std::vector< glm::vec3 > normals = {},
        std::vector< glm::vec2 > textures = {});
    glm::mat4 GetMMatrix(glm::mat4 startMatrix = glm::mat4(1.0f), glm::vec3 scaleVec = glm::vec3(2, 2, 2));
};


