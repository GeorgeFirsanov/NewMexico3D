#include "Model.h"
/*#include "func.h"
#include "globals.h"*/

void Model::render(GLfloat* Mmatr, GLfloat* Vmatr, GLfloat* Pmatr, GLuint mode)
{
    //Выбираем шейдерную программу
    glUseProgram(shader_programme);
    
    GLint lightColorLoc = glGetUniformLocation(shader_programme, "lightColor");

    GLint lightDir = glGetUniformLocation(shader_programme, "light.direction");
    GLint lightAm = glGetUniformLocation(shader_programme, "light.ambient");
    GLint lightDif = glGetUniformLocation(shader_programme, "light.diffuse");
    GLint lightSp = glGetUniformLocation(shader_programme, "light.specular");

    GLint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

    glUniform3f(lightDir, 0.1f, -0.2f, 0.3f);
    glUniform3f(lightAm, ambient.x, ambient.y, ambient.z);
    glUniform3f(lightDif, diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(lightSp, specular.x, specular.y, specular.z);

    glUniform3f(viewPosLoc, cam.Position.x, cam.Position.y, cam.Position.z);
    
    GLuint Mmatrix = glGetUniformLocation(shader_programme, "model");
    GLuint Vmatrix = glGetUniformLocation(shader_programme, "view");
    GLuint Pmatrix = glGetUniformLocation(shader_programme, "projection");
    glUniformMatrix4fv(Mmatrix, 1, GL_FALSE, Mmatr);
    glUniformMatrix4fv(Vmatrix, 1, GL_FALSE, Vmatr);
    glUniformMatrix4fv(Pmatrix, 1, GL_FALSE, Pmatr);
    //Активируем массив вершин
    glBindVertexArray(vao);
    //Если индексы есть - рисуем по ним
    if(mode == -1)
        if (indices_count > 0)
            glDrawElements(this->mode, indices_count, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(this->mode, 0, verteces_count);
    else
        if (indices_count > 0)
            glDrawElements(mode, indices_count, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(mode, 0, verteces_count);
}

void Model::set_mode(GLuint new_mode)
{
    mode = new_mode;
}

void Model::load_coords(glm::vec3* verteces, size_t count)
{
    //Сохранение размера массива
    verteces_count = count;

    //Создание VBO
    GLuint coords_vbo = 0;
    glGenBuffers(1, &coords_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, coords_vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), verteces, GL_STATIC_DRAW);

    //Так как VAO уже создан, то можно сразу связать с ним
    glBindVertexArray(vao);

    //Собственно связь
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); //положение

    //Подключение атрибута
    glEnableVertexAttribArray(0); //position attribute
}

void Model::load_normals(glm::vec3* normals, size_t count)
{
    //Создание VBO
    GLuint norm_vbo = 0;
    glGenBuffers(1, &norm_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), normals, GL_STATIC_DRAW);

    //Так как VAO уже создан, то можно сразу связать с ним
    glBindVertexArray(vao);

    //Собственно связь
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); //нормали

    //Подключение атрибута
    glEnableVertexAttribArray(1); //normals attribute
}

void Model::load_colors(glm::vec3* colors, size_t count)
{
    //Аналогично координатам
    //Размер сохранять не требуется - важно только число вершин
    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), colors, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(2);
}

void Model::load_indices(GLuint* indices, size_t count)
{
    //Сохранение числа индексов
    indices_count = count;

    //Дальше всё почти как всегда с поправкой на тип буфера
    glBindVertexArray(vao);

    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer); // Генерация одного объекта буфера вершин
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer); // Привязка элементного буфера
    //Загрузка индексов в используемый элементный буфер
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);

}

void Model::load_shaders(const char* vect, const char* frag)
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
