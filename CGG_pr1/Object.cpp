#include "Object.h"

Object::Object( const char* pth, glm::vec3 Position) : Object(Position)
{
    if (!loadOBJ(pth, vertices, textures, normals))
        std::cout << "loading OBJ error";
    //genSphere(indices, vertices, textures, normals, 0.9f, 13, 13);
    size_t size = vertices.size();
    //генерация цветов
    for (int i = 0; i < size; i++)
    {
        colors.push_back(glm::vec3((1 - 0.0001 * i), 0.00001 * i, 0.0002 * i));
    }
    _LoadAll();
}

Object::Object(glm::vec3 Position)
{
    this->position = Position;
}
glm::mat4 Object::GetMMatrix(glm::mat4 startMatrix, glm::vec3 scaleVec)
{
    glm::mat4 MMatr = startMatrix;
    MMatr = glm::translate(MMatr, this->position);  // Перемещение модели
    MMatr = glm::rotate(MMatr, glm::radians(this->degree), glm::vec3(0, 1, 0));  // Вращение модели
    MMatr = glm::scale(MMatr, scaleVec);  // Масштабирование модели
    return MMatr;
}

int Object::Setter(std::vector< glm::vec3 > vertices,
    std::vector< GLuint >    indices,
    std::vector< glm::vec3 > colors,
    std::vector< glm::vec3 > normals,
    std::vector< glm::vec2 > textures)
{
    if (vertices.empty())
        return 1;
    size_t size = vertices.size();
    this->vertices = vertices;
    this->indices = indices;

    if (!textures.empty() && textures.size() != size)
        return 3;
    this->textures = textures;
    this->normals = normals;

    if (!colors.empty() && colors.size() != size)
        return 5;
    this->colors = colors;

    _LoadAll();
    return 0;
}

void Object::_LoadAll()
{
    size_t size = vertices.size();
    load_indices(indices.data(), indices.size());
    load_normals(normals.data(), normals.size());
    load_coords(vertices.data(), size);
    load_colors(colors.data(), size);

}
