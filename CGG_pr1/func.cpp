
#include "func.h"
#include "Globals.h"


string LoadShader(const char* filename)
{
    string res;
    ifstream file(filename, ios::in);
    if (file.is_open()) {
        std::stringstream sstr; // Буфер для чтения
        sstr << file.rdbuf(); // Считываем файл
        res = sstr.str();   //Получаем строку из буфера
        file.close();       //Закрываем файл
    }
    return res;
}

GLFWwindow* InitAll(int w, int h, bool Fullscreen)
{
    //В глобальные переменные сохраняем размеры окна
    WinWidth = w;
    WinHeight = h;

    //Создаём переменную для хранения ID окна
    GLFWwindow* window = nullptr;

    //Подключение GLFW
    if (!glfwInit()) {
        cerr << "ERROR: could not start GLFW3\n";
        exit(-1);
    }

    //В зависимости от режима экрана создаётся окно с заголовком Capybara OpenGL
    if (Fullscreen)
    {

        GLFWmonitor* mon = glfwGetPrimaryMonitor();
        const GLFWvidmode* vmode = glfwGetVideoMode(mon);
        WinWidth = vmode->width;
        WinHeight = vmode->height;
        window = glfwCreateWindow(WinWidth, WinHeight, "New Mexico", mon, NULL);

    }
    else
        window = glfwCreateWindow(WinWidth, WinHeight, "New Mexico", NULL, NULL);

    //Привязываются функции для обработки событий
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    //Активируется контекст окна
    glfwMakeContextCurrent(window);

    //Подключение новейшей из доступных версий OpenGL
    glewExperimental = GL_TRUE;

    //Подключение функций OpenGL
    if (glewInit() != GLEW_OK) {
        cerr << "ERROR: could not start GLEW\n";
        return nullptr;
    }

    //Если дошли до сюда - возвращаем окно
    return window;
}

void EndAll()
{
    //Просто выключение GLFW
    //Сюда же можно добавить очистку памяти если будет нужно
    glfwTerminate();
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    //Просто меняем глобальные переменные
    WinWidth = width;
    WinHeight = height;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        cam.ProcessKeyboard(FORWARD, 0.05);
    }
    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        cam.ProcessKeyboard(BACKWARD, 0.05);
    }
    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        cam.ProcessKeyboard(LEFT, 0.05);
    }
    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        cam.ProcessKeyboard(RIGHT, 0.05);
    }
    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
        ModelDegree-=3;
    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
        ModelDegree+=3;
    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
        ModelPosition.x -= ModelSpeed;
    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
        ModelPosition.x += ModelSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    cam.ProcessMouseMovement(xpos, ypos, true);
}