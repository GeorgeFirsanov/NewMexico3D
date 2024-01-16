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
    /// ������� ����������� - ������ ������ ������, ���� ������. � ���������� ����.
    /// </summary>
    /// <param name="w"></param>
    Model() {
        glGenVertexArrays(1, &vao);
    };

    //����������. ��� ��� ������������ ������ ���, �� �� ������ �� ������.
    ~Model() {};


    //����� ��� ����������� ������.     
    /// <summary>
    /// ������ ����� ��� ���������� - ������ ���������� ������ ������.
    /// </summary>
    /// <param name="mode">������������ �������� - ����� ���������.</param>
    virtual void render(GLfloat* Mmatr, GLfloat* Vmatr, GLfloat* Pmatr,  GLuint mode = -1);

    //����� ��������� ������ ���������
    void set_mode(GLuint mode = GL_TRIANGLES);

    //����� ������� ��� �������� ������������ ������� ������
    //� ���������� ���������� ����� ��������� ����� �������

    /// <summary>
    /// ����� ��� �������� ��������� ������.
    /// </summary>
    /// <param name="verteces">������ � ������������.</param>
    /// <param name="count">������ �������.</param>
    void load_coords(glm::vec3* verteces, size_t count);
    /// <summary>
    /// ����� ��� �������� �������� ������.
    /// </summary>
    /// <param name="normals">������ � ������������.</param>
    /// <param name="count">������ �������.</param>
    void load_normals(glm::vec3* normals, size_t count);
    /// <summary>
    /// ����� ��� �������� ������ ������.
    /// </summary>
    /// <param name="colors">������ ������.</param>
    /// <param name="count">������ �������.</param>
    void load_colors(glm::vec3* colors, size_t count);

    /// <summary>
    /// ����� ��� �������� ������� ��������.
    /// </summary>
    /// <param name="indices">������ ��������.</param>
    /// <param name="count">������ �������.</param>
    void load_indices(GLuint* indices, size_t count);

    /// <summary>
    /// ����� ��� �������� ��������. � ����� ������� ��������� ������ ��������� � ����������� �������
    /// � ���������� ���������� ����� ������������ ��������� ���������.
    /// </summary>
    /// <param name="vect">���� � ���������� �������</param>
    /// <param name="frag">���� � ������������ �������</param>
    void load_shaders(const char* vect, const char* frag);

    glm::vec3 ambient = { 0.1f, 0.1f, 0.1f };
    glm::vec3 diffuse = { 0.8f, 0.8f, 0.8f };
    glm::vec3 specular = { 1.0f, 1.0f, 1.0f };

private:

    /// <summary>
    /// ID ������� ������
    /// </summary>
    GLuint vao = -1;    //��������� �������� - ������������ �����, �������� �����������
    /// <summary>
    /// ���������� ������
    /// </summary>
    size_t verteces_count = 0;
    /// <summary>
    /// ���������� ��������
    /// </summary>
    size_t indices_count = 0;

    /// <summary>
    /// ID ��������� ���������
    /// </summary>
    GLuint shader_programme = -1;//��������� �������� - ������������ �����, �������� �����������

    /// <summary>
    /// ����� ���������
    /// </summary>
    GLuint mode = GL_TRIANGLES;
};
