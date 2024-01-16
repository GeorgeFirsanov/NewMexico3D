#pragma once
#include "Model.h"
#include "globals.h"

bool loadOBJ(const char* path,                       //Путь к файлу
    std::vector<glm::vec3>& outVertices,    //Выходной массив координат
    std::vector<glm::vec2>& outTextures,    //Выходной массив текстурных координат
    std::vector<glm::vec3>& outNormals      //Выходной массив нормалей
);

void genSphere(
    std::vector<GLuint>& outIndexes,        //Выходной массив индексов
    std::vector < glm::vec3 >& outVertices, //Выходной массив координат
    std::vector < glm::vec2 >& outTextures, //Выходной массив текстурных координат
    std::vector < glm::vec3 >& outNormals,  //Выходной массив векторов нормалей
    float radius,                           //Радиус сферы
    int sectorCount,                        //Число меридианов
    int stackCount                          //Число параллелей
);