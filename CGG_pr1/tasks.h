#pragma once
#include "Model.h"
#include "globals.h"

bool loadOBJ(const char* path,                       //���� � �����
    std::vector<glm::vec3>& outVertices,    //�������� ������ ���������
    std::vector<glm::vec2>& outTextures,    //�������� ������ ���������� ���������
    std::vector<glm::vec3>& outNormals      //�������� ������ ��������
);

void genSphere(
    std::vector<GLuint>& outIndexes,        //�������� ������ ��������
    std::vector < glm::vec3 >& outVertices, //�������� ������ ���������
    std::vector < glm::vec2 >& outTextures, //�������� ������ ���������� ���������
    std::vector < glm::vec3 >& outNormals,  //�������� ������ �������� ��������
    float radius,                           //������ �����
    int sectorCount,                        //����� ����������
    int stackCount                          //����� ����������
);