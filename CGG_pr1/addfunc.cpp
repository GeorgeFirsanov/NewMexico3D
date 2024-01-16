#include "tasks.h"
#include <algorithm>

/*
    * �������� ����� obj    loadOBJ
    * �������� ����         genSphere
*/
bool loadOBJ( const char* path,                       //���� � �����
    std::vector<glm::vec3>& outVertices,    //�������� ������ ���������
    std::vector<glm::vec2>& outTextures,    //�������� ������ ���������� ���������
    std::vector<glm::vec3>& outNormals      //�������� ������ ��������
)
{
    //��������� ������� ��� �������� �������� � �������� �� �����
    std::vector< unsigned int > vertexIndices, vtIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_textures;
    std::vector< glm::vec3 > temp_normals;

    //������ ��� �������� ������� �� �����
    string buf;
    //�����, �������� ���������� �����
    std::stringstream sstr;
    //���������� ��� ���������� ������ � ����������� ������ ������ � �����
    bool cont = false;

    //���������� �����
    ifstream file(path, ios::in);
    if (file.is_open()) {
        sstr << file.rdbuf();
        file.close();
    }
    else
        return false;

    //������� �������� ��������
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    //������� ������� ����� �� �����
    while (cont || sstr >> buf) {
        cont = false;

        //��������� ���������
        if (buf == "v") {
            glm::vec3 vertex;
            sstr >> vertex.x;
            sstr >> vertex.y;
            sstr >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        //��������� ���������� ���������
        else if (buf == "vt") {
            glm::vec2 texture;
            sstr >> texture.x;
            sstr >> texture.y;
            temp_textures.push_back(texture);
        }
        //��������� ��������
        else if (buf == "vn") {
            glm::vec3 normal;
            sstr >> normal.x;
            sstr >> normal.y;
            sstr >> normal.z;
            temp_normals.push_back(normal);
        }
        //��������� ������
        else if (buf == "f") {
            //��������� ������� ��� �������� ���������� � �����
            vector<string> vertexes;
            vector<int> vertindexes;
            while (!cont)
            {
                //����������� �����
                sstr >> buf;
                //���� ����� �������� ������ � ������� - ��� ������������ � ������
                if (buf.find('/') != string::npos)
                    vertexes.push_back(buf);
                //���� ����� - ������ ��������� ������, �� ����������� � ���������� ��������� �����
                else
                {
                    //��� ������ �� ���������� ������ ������������ ������� ������
                    for (string vert : vertexes)
                    {
                        std::replace(vert.begin(), vert.end(), '/', ' ');
                        std::stringstream tmpstream(vert);
                        int v, vt, n;
                        tmpstream >> v;
                        tmpstream >> vt;
                        tmpstream >> n;
                        //������� ��������� � ��������� ������
                        vertindexes.push_back(v);
                        vertindexes.push_back(vt);
                        vertindexes.push_back(n);
                    }
                    //������ ��� ������� ��������� �������� ��������
                    for (int i = 0; i < 3; i++)
                    {
                        vertexIndices.push_back(vertindexes[i * 3 + 0]);
                        vtIndices.push_back(vertindexes[i * 3 + 1]);
                        normalIndices.push_back(vertindexes[i * 3 + 2]);
                    }
                    //������ ������� - ���� ��� �������� �������, ���� � �� �������
                    //�� ���� ������������� �� ����� ������������� � ����� �������������
                    size_t tmpsize = vertexes.size();
                    if (tmpsize > 3)
                    {
                        //��� ������ �� ������ ����������� ��� �������, ���������� �����������
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
                    //����� �� ��������� ��������� �����, �������� ���� � ��� ��� �� �������
                    cont = true;
                }
            }
        }
    }
    // ������ ��������� �������� �������� � �������� �������� ��������
    // ��� ������ ������� � ������� ��������, �������������� �������
    // ��� ��������� �������� ��������� � �������� ������

    size_t tmpsize = vertexIndices.size();
    for (unsigned int i = 0; i < tmpsize; i++)
    {
        //���� ������ ��� ������� � ����� ���������� � 1!
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
    std::vector<GLuint>& outIndexes,        //�������� ������ ��������
    std::vector < glm::vec3 >& outVertices, //�������� ������ ���������
    std::vector < glm::vec2 >& outTextures, //�������� ������ ���������� ���������
    std::vector < glm::vec3 >& outNormals,  //�������� ������ �������� ��������
    float radius,                           //������ �����
    int sectorCount,                        //����� ����������
    int stackCount                          //����� ����������
)
{
    //������� �������� ��������
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    //��������������� ���������� ��� �������� ������������� ������
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;
    float sectorStep = 2.0f * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    //���� �� ������ ���������
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // ������� �� PI/2 � �� -PI/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // �� ������ ��������� ����������� (sectorCount+1) ������
        // ��� ������ � ��������� ��������� ������� � �������, �� ���������� ���������� ����������
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // �� 0 �� 2PI

            // ������������� ���������� (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            glm::vec3 vert;
            vert.x = x;
            vert.y = y;
            vert.z = z;
            outVertices.push_back(vert);

            // ������������� ������ ������� (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            glm::vec3 norm;
            norm.x = nx;
            norm.y = ny;
            norm.z = nz;
            outNormals.push_back(norm);

            // ������������� ���������� ���������� (s, t) � ��������� [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            glm::vec2 vt;
            vt.s = s;
            vt.t = t;
            outTextures.push_back(vt);
        }

        //�� ��������� ���� - ����� ������� ������, �.�. �������
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // ������ �������� ���������
            k2 = k1 + sectorCount + 1;      // ������ ���������� ���������

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // ��� ������ � ��������� ��������� �� 1 ������������, ��� ��������� - �� ���
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
