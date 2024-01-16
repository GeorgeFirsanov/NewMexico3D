
#include "func.h"
#include "Globals.h"


string LoadShader(const char* filename)
{
    string res;
    ifstream file(filename, ios::in);
    if (file.is_open()) {
        std::stringstream sstr; // ����� ��� ������
        sstr << file.rdbuf(); // ��������� ����
        res = sstr.str();   //�������� ������ �� ������
        file.close();       //��������� ����
    }
    return res;
}

GLFWwindow* InitAll(int w, int h, bool Fullscreen)
{
    //� ���������� ���������� ��������� ������� ����
    WinWidth = w;
    WinHeight = h;

    //������ ���������� ��� �������� ID ����
    GLFWwindow* window = nullptr;

    //����������� GLFW
    if (!glfwInit()) {
        cerr << "ERROR: could not start GLFW3\n";
        exit(-1);
    }

    //� ����������� �� ������ ������ �������� ���� � ���������� Capybara OpenGL
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

    //������������� ������� ��� ��������� �������
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    //������������ �������� ����
    glfwMakeContextCurrent(window);

    //����������� �������� �� ��������� ������ OpenGL
    glewExperimental = GL_TRUE;

    //����������� ������� OpenGL
    if (glewInit() != GLEW_OK) {
        cerr << "ERROR: could not start GLEW\n";
        return nullptr;
    }

    //���� ����� �� ���� - ���������� ����
    return window;
}

void EndAll()
{
    //������ ���������� GLFW
    //���� �� ����� �������� ������� ������ ���� ����� �����
    glfwTerminate();
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    //������ ������ ���������� ����������
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