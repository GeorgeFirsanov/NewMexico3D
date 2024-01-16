#pragma once
#include "Camera.h"
#include <glm/glm.hpp>
//����� ���������� ���������� � �������� ��� ���������� ������

#define PI 3.14159265358979323846f  

/// <summary>
/// ������ ����.
/// </summary>
extern int WinWidth;
/// <summary>
/// ������ ����.
/// </summary>
extern int WinHeight;
/// <summary>
/// ��������� �������. ��� �������� ������ ��������.
/// </summary>
extern const glm::mat4 WorldMatrix;
/// <summary>
/// ����������� �������� ������.
/// </summary>
extern bool rotationmode;


extern glm::vec3 ModelPosition;
extern GLfloat radius;
extern const GLfloat ModelSpeed;
extern GLfloat ModelDegree;

extern Camera cam;