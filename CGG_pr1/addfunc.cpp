#include "tasks.h"
#include <algorithm>

/*
    * Загрузка файла obj    loadOBJ
    * Создание шара         genSphere
*/
bool loadOBJ( const char* path,                       //Путь к файлу
    std::vector<glm::vec3>& outVertices,    //Выходной массив координат
    std::vector<glm::vec2>& outTextures,    //Выходной массив текстурных координат
    std::vector<glm::vec3>& outNormals      //Выходной массив нормалей
)
{
    //Временные массивы для хранения индексов и значений из файла
    std::vector< unsigned int > vertexIndices, vtIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_textures;
    std::vector< glm::vec3 > temp_normals;

    //Строка для хранения токенов из файла
    string buf;
    //Буфер, хранящий содержимое файла
    std::stringstream sstr;
    //Переменная для корректной работы с неизвестным числом вершин в грани
    bool cont = false;

    //Считывание файла
    ifstream file(path, ios::in);
    if (file.is_open()) {
        sstr << file.rdbuf();
        file.close();
    }
    else
        return false;

    //Очистка выходных массивов
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    //Попытка считать токен из файла
    while (cont || sstr >> buf) {
        cont = false;

        //Обработка координат
        if (buf == "v") {
            glm::vec3 vertex;
            sstr >> vertex.x;
            sstr >> vertex.y;
            sstr >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        //Обработка текстурных координат
        else if (buf == "vt") {
            glm::vec2 texture;
            sstr >> texture.x;
            sstr >> texture.y;
            temp_textures.push_back(texture);
        }
        //Обработка нормалей
        else if (buf == "vn") {
            glm::vec3 normal;
            sstr >> normal.x;
            sstr >> normal.y;
            sstr >> normal.z;
            temp_normals.push_back(normal);
        }
        //Обработка граней
        else if (buf == "f") {
            //Временные массивы для хранения информации о грани
            vector<string> vertexes;
            vector<int> vertindexes;
            while (!cont)
            {
                //Считывается токен
                sstr >> buf;
                //Если токен содержит данные о вершине - она записывается в массив
                if (buf.find('/') != string::npos)
                    vertexes.push_back(buf);
                //Если токен - начало следующей строки, он сохраняется и начинается обработка грани
                else
                {
                    //Для каждой из сохранённых вершин производится парсинг данных
                    for (string vert : vertexes)
                    {
                        std::replace(vert.begin(), vert.end(), '/', ' ');
                        std::stringstream tmpstream(vert);
                        int v, vt, n;
                        tmpstream >> v;
                        tmpstream >> vt;
                        tmpstream >> n;
                        //Индексы заносятся в временный массив
                        vertindexes.push_back(v);
                        vertindexes.push_back(vt);
                        vertindexes.push_back(n);
                    }
                    //Первые три вершины заносятся вмассивы индексов
                    for (int i = 0; i < 3; i++)
                    {
                        vertexIndices.push_back(vertindexes[i * 3 + 0]);
                        vtIndices.push_back(vertindexes[i * 3 + 1]);
                        normalIndices.push_back(vertindexes[i * 3 + 2]);
                    }
                    //Дальше сложнее - если ещё остались вершины, надо и их занести
                    //Но надо преобразовать из веера треугольников в набор треугольников
                    size_t tmpsize = vertexes.size();
                    if (tmpsize > 3)
                    {
                        //Для каждой из вершин добавляются три вершины, образующие треугольник
                        for (int i = 3; i < tmpsize; i++)
                        {
                            vertexIndices.push_back(vertindexes[0]);
                            vtIndices.push_back(vertindexes[1]);
                            normalIndices.push_back(vertindexes[2]);
                            vertexIndices.push_back(vertindexes[(i - 1) * 3 + 0]);
                            vtIndices.push_back(vertindexes[(i - 1) * 3 + 1]);
                            normalIndices.push_back(vertindexes[(i - 1) * 3 + 2]);
                            vertexIndices.push_back(vertindexes[i * 3 + 0]);
                            vtIndices.push_back(vertindexes[i * 3 + 1]);
                            normalIndices.push_back(vertindexes[i * 3 + 2]);
                        }
                    }
                    //Чтобы не потерялся считанный токен, ставится флаг о том что он сохранён
                    cont = true;
                }
            }
        }
    }
    // Теперь обработка массивов индексов и создание выходных массивов
    // Для каждой вершины в массиве значения, соответсвующие вершине
    // под указанным индексом заносятся в выходной массив

    size_t tmpsize = vertexIndices.size();
    for (unsigned int i = 0; i < tmpsize; i++)
    {
        //Надо учесть что индексы в файле начинаются с 1!
        glm::vec3 vertex = temp_vertices[vertexIndices[i] - 1];
        outVertices.push_back(vertex);
        glm::vec3 normal = temp_normals[normalIndices[i] - 1];
        outNormals.push_back(normal);
        glm::vec2 vt = temp_textures[vtIndices[i] - 1];
        outTextures.push_back(vt);
    }
    return true;
}

void genSphere(
    std::vector<GLuint>& outIndexes,        //Выходной массив индексов
    std::vector < glm::vec3 >& outVertices, //Выходной массив координат
    std::vector < glm::vec2 >& outTextures, //Выходной массив текстурных координат
    std::vector < glm::vec3 >& outNormals,  //Выходной массив векторов нормалей
    float radius,                           //Радиус сферы
    int sectorCount,                        //Число меридианов
    int stackCount                          //Число параллелей
)
{
    //Очистка выходных массивов
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    //Вспомогательные переменные для хранения промежуточных данных
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;
    float sectorStep = 2.0f * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    //Цикл по каждой параллели
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // начиная от PI/2 и до -PI/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // На каждую параллель добавляется (sectorCount+1) вершин
        // для первой и последней совпадают позиция и нормаль, но отличаются текстурные координаты
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // от 0 до 2PI

            // высчитываются координаты (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            glm::vec3 vert;
            vert.x = x;
            vert.y = y;
            vert.z = z;
            outVertices.push_back(vert);

            // высчитывается вектор нормали (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            glm::vec3 norm;
            norm.x = nx;
            norm.y = ny;
            norm.z = nz;
            outNormals.push_back(norm);

            // высчитываются текстурные координаты (s, t) в диапазоне [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            glm::vec2 vt;
            vt.s = s;
            vt.t = t;
            outTextures.push_back(vt);
        }

        //Но координат мало - нужен порядок обхода, т.е. индексы
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // начало текущего меридиана
            k2 = k1 + sectorCount + 1;      // начало следующего меридиана

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // Для первой и последней параллели по 1 треугольнику, для остальных - по два
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    outIndexes.push_back(k1);
                    outIndexes.push_back(k2);
                    outIndexes.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    outIndexes.push_back(k1 + 1);
                    outIndexes.push_back(k2);
                    outIndexes.push_back(k2 + 1);
                }
            }
        }
    }
}
