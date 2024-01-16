#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "func.h"
#include "globals.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Model
{
public:
    /// <summary>
    /// Простой конструктор - создаёт массив вершин, пока пустой. И подключает окно.
    /// </summary>
    /// <param name="w"></param>
    Model() {
        glGenVertexArrays(1, &vao);
    };

    //Деструктор. Так как динамической памяти нет, то он ничего не делает.
    ~Model() {};


    //Метод для отображения модели.     
    /// <summary>
    /// Рендер почти без параметров - просто отображает массив вершин.
    /// </summary>
    /// <param name="mode">Единственный параметр - режим рисования.</param>
    virtual void render(GLfloat* Mmatr, GLfloat* Vmatr, GLfloat* Pmatr,  GLuint mode = -1);

    //Метод установки режима рисования
    void set_mode(GLuint mode = GL_TRIANGLES);

    //Набор методов для загрузки составляющих массива вершин
    //В дальнейшем аналогично стоит добавлять новые функции

    /// <summary>
    /// Метод для загрузки координат вершин.
    /// </summary>
    /// <param name="verteces">Массив с координатами.</param>
    /// <param name="count">Размер массива.</param>
    void load_coords(glm::vec3* verteces, size_t count);
    /// <summary>
    /// Метод для загрузки нормалей вершин.
    /// </summary>
    /// <param name="normals">Массив с координатами.</param>
    /// <param name="count">Размер массива.</param>
    void load_normals(glm::vec3* normals, size_t count);
    /// <summary>
    /// Метод для загрузки цветов вершин.
    /// </summary>
    /// <param name="colors">Массив цветов.</param>
    /// <param name="count">Размер массива.</param>
    void load_colors(glm::vec3* colors, size_t count);

    /// <summary>
    /// Метод для загрузки массива индексов.
    /// </summary>
    /// <param name="indices">Массив индексов.</param>
    /// <param name="count">Размер массива.</param>
    void load_indices(GLuint* indices, size_t count);

    /// <summary>
    /// Метод для загрузки шейдеров. С целью примера загружает только вершинный и фрагментный шейдеры
    /// В результате выполнения будет скомпонована шейдерная программа.
    /// </summary>
    /// <param name="vect">Путь к вершинному шейдеру</param>
    /// <param name="frag">Путь к фрагментному шейдеру</param>
    void load_shaders(const char* vect, const char* frag);

    glm::vec3 ambient = { 0.1f, 0.1f, 0.1f };
    glm::vec3 diffuse = { 0.8f, 0.8f, 0.8f };
    glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

private:

    /// <summary>
    /// ID массива вершин
    /// </summary>
    GLuint vao = -1;    //Начальное значение - максимальное число, заведомо невозможное
    /// <summary>
    /// Количество вершин
    /// </summary>
    size_t verteces_count = 0;
    /// <summary>
    /// Количество индексов
    /// </summary>
    size_t indices_count = 0;

    /// <summary>
    /// ID шейдерной программы
    /// </summary>
    GLuint shader_programme = -1;//Начальное значение - максимальное число, заведомо невозможное

    /// <summary>
    /// Режим рисования
    /// </summary>
    GLuint mode = GL_TRIANGLES;
};
