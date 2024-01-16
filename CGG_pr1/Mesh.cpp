#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

void Mesh::render(GLuint shader_programme, GLfloat* Mmatr, GLfloat* Vmatr, GLfloat* Pmatr)
{
    glUseProgram(shader_programme);
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    std::string number;
    std::string name;
    name.reserve(16); //optimization 
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        TexType type = textures[i].type;
        switch (type)
        {
        case TexType::TEXTURE_DIFFUSE:
            number = std::to_string(diffuseNr++);
            name = "texture_diffuse";
            break;
        case TexType::TEXTURE_SPECULAR:
            number = std::to_string(specularNr++); // unsigned int to string
            name = "texture_specular";
            break;
        case TexType::TEXTURE_NORMAL:
            number = std::to_string(normalNr++); // unsigned int to string
            name = "texture_normal";
            break;
        case TexType::TEXTURE_HEIGHT:
            number = std::to_string(heightNr++); // unsigned int to string
            name = "texture_height";
            break;
        }

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader_programme, (name + number).c_str()), i);

        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "model"), 1, GL_FALSE, Mmatr);
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "view"), 1, GL_FALSE, Vmatr);
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "projection"), 1, GL_FALSE, Pmatr);
        
        GLint lightDir = glGetUniformLocation(shader_programme, "light.direction");
        GLint lightAm = glGetUniformLocation(shader_programme, "light.ambient");
        GLint lightDif = glGetUniformLocation(shader_programme, "light.diffuse");
        GLint lightSp = glGetUniformLocation(shader_programme, "light.specular");
        GLint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");

        glUniform3f(lightDir, 0.3f, -0.2f, -0.1f);
        glUniform3f(lightAm, 0.2, 0.1, 0.2);
        glUniform3f(lightDif, 0.8, 0.7, 0.8);
        glUniform3f(lightSp, 0.8, 0.8, 0.8);

        glUniform3f(viewPosLoc, cam.Position.x, cam.Position.y, cam.Position.z);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
}


void Model3D::Draw(GLfloat* Mmatr, GLfloat* Vmatr, GLfloat* Pmatr)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].render(shader_programme, Mmatr, Vmatr, Pmatr);
}

void Model3D::loadModel(std::string path)
{
    Assimp::Importer import;
    //вызов загрузчика файла               путь, преобразование в треуг| инверсия оY где надо
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // указатель сцены, указатель корневого узла и флаг проверяются
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << " in Model3D::loadModel(path)" << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    //рекурсивная обработка всех узлов иерархии
    //можно и без этого, но для выстраивания иерархии "родитель-потомок"
    processNode(scene->mRootNode, scene);
}

void Model3D::processNode(aiNode* node, const aiScene* scene)
{
    // обработать все полигональные сетки в узле(если есть)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // выполнить ту же обработку и для каждого потомка узла
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model3D::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // обработка координат, нормалей и текстурных координат вершин
        glm::vec3 vector; //вспомогательный вектор
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // проверка на наличие текстурных координат
        if (mesh->mTextureCoords[0]) 
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        
        vertices.push_back(vertex);
    }
    // обработка индексов
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // обработка материала
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TexType::TEXTURE_DIFFUSE);
        textures.insert(textures.end( ), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TexType::TEXTURE_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, TexType::TEXTURE_NORMAL);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model3D::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TexType typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}

unsigned int Model3D::TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model3D::loadShaders(const char* vect, const char* frag)
{
    // Переменные под результат компиляции программы
    GLint result = GL_FALSE;
    int infoLogLength;

    //Создание шейдерной программы
    shader_programme = glCreateProgram();

    //Загрузка текстов шейдеров из файлов
    string vstext = LoadShader(vect);
    const char* vertex_shader = vstext.c_str();
    string fstext = LoadShader(frag);
    const char* fragment_shader = fstext.c_str();

    //Создание вершинного шейдера
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    //Проверка результата компиляции
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);
    //Вывод сообщения об ошибке если что-то пошло не так
    if (infoLogLength > 0)
    {
        char* errorMessage = new char[infoLogLength + 1];
        glGetShaderInfoLog(vs, infoLogLength, NULL, errorMessage);
        std::cout << "vertex shader: " << errorMessage;
        delete errorMessage;
    }

    //Аналогично с фрагментным шейдером
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        char* errorMessage = new char[infoLogLength + 1];
        glGetShaderInfoLog(fs, infoLogLength, NULL, errorMessage);
        std::cout << "fragment shader: " << errorMessage;
        delete errorMessage;
    }

    //Сборка программы
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);

    //Компоновка шейдерной программы
    glLinkProgram(shader_programme);
}

glm::mat4 Model3D::GetMMatrix(glm::vec3 scaleVec, glm::mat4 startMatrix)
{
    glm::mat4 MMatr = startMatrix;
    MMatr = glm::translate(MMatr, this->position);  // Перемещение модели
    MMatr = glm::rotate(MMatr, glm::radians(this->degree), glm::vec3(0, 0, 1));  // Вращение модели
    MMatr = glm::scale(MMatr, scaleVec);  // Масштабирование модели
    return MMatr;
}