#pragma once
#include "stb_image.h"

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <iostream>

#include "func.h"
#include "globals.h"


#define MAX_BONE_INFLUENCE 4

/// <summary>
/// Структура для  хранения вершины
/// </summary>
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};
/// <summary>
/// Перечисление всех возможных типов текстур
/// </summary>
enum class TexType
{
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT
};

/// <summary>
/// Структура для хранения текстуры
/// </summary>
struct Texture {
    unsigned int id;
    TexType type;
    std::string path;
};

/// <summary>
/// Класс элементарной сетки
/// Используется для загрузки данных из структур 
/// библиотеки Assimp
/// </summary>
class Mesh
{
public:
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;

    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // render the mesh
    void render(GLuint shader_programme, GLfloat* Mmatr, GLfloat* Vmatr, GLfloat* Pmatr);

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};

/// <summary>
/// Класс для хранения моделей.
/// Используется для загрузки данных из структур
/// библиотеки Assimp
/// Содержит массив элементарных сеток
/// </summary>
class Model3D
{
public:
    Model3D(const char* path, glm::vec3 Position)
    {
        this->position = Position;
        loadModel(path);
    }
    void Draw(GLfloat* Mmatr, GLfloat* Vmatr, GLfloat* Pmatr);
    void loadShaders(const char* vect, const char* frag);
    glm::mat4 GetMMatrix(glm::vec3 scaleVec = glm::vec3(1, 1, 1), glm::mat4 startMatrix = glm::mat4(1.0f));
    GLfloat degree = 0;
    glm::vec3 position = glm::vec3(0);
private:
    std::vector<Mesh> meshes;
    std::string directory;
    GLuint shader_programme;

    void loadModel(std::string path);
    void processNode(aiNode * node, const aiScene * scene);
    Mesh processMesh(aiMesh * mesh, const aiScene * scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type,
        TexType typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory);
};