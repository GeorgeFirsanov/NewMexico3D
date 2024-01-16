/*
    COURSE WORK on OpenGL by George Firsanov

        """ NEW MEXICO. COOKING """

    ~ The scene of RV in New Mexico desert 
                    with 3D objects and sound effects ~

    Here are some libs (open source and free):
    - OpenGl
    - stb_image
    - glfw
    - glm
    - Assimp
    - SFML (Audio)


    01.2024
*/

#include "Mesh.h"
#include "func.h"
#include "windows.h"
#include "globals.h"

int WinWidth;
int WinHeight;
const glm::mat4 WorldMatrix = glm::mat4(1.0f); 

glm::vec3 ModelPosition = { -5, 0.7f, 6.3 };
GLfloat const ModelSpeed = 0.01;
GLfloat ModelDegree = 0;

Camera cam({ -4, 1.5, 5.f }, ModelPosition, 0, 0);

int main()
{
    GLFWwindow* window = InitAll(1724, 768, false);
    
    //Улавливание мыши
    //Прячем курсор
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    if (window == nullptr)
    {
        EndAll();
        return -1;
    }
    /// Models initializing///
    Model3D rv("./models/textures/rv/RV.obj", { 1.f, .35f, 5.f });

    Model3D brunch1("./models/textures/items/brunch1.obj", ModelPosition);
    Model3D brunch2("./models/textures/items/brunch2.obj", { -1, 0.5f, 0 });
    Model3D brunch3("./models/textures/items/brunch3.obj", { -4, 0.7f, 6 });

    Model3D chair1("./models/textures/items/chair.obj", { -2.5, 0.65f, 4 });
    Model3D chair2("./models/textures/items/chair.obj", { -3, 0.65f, 3.5 });
    chair1.degree = 5; //changing angle
    Model3D floor("./models/textures/nature/ground2.obj", { 0, 0.5, 0 });
    Model3D sky("./models/textures/nature/sky.obj", {0, -10, 0});

    //loading shaders
    rv.loadShaders("vs2.glsl", "fs2.glsl");
    brunch1.loadShaders("vs2.glsl", "fs2.glsl");
    brunch2.loadShaders("vs2.glsl", "fs2.glsl");
    brunch3.loadShaders("vs2.glsl", "fs2.glsl");
    chair1.loadShaders("vs2.glsl", "fs2.glsl");
    chair2.loadShaders("vs2.glsl", "fs2.glsl");
    rv.degree = 5;
    floor.loadShaders("vs2.glsl", "fs2.glsl");
    sky.loadShaders("vs.glsl", "fs.glsl");
    
    ///Sounds and music///
    sf::Sound steps;
    sf::SoundBuffer sound_buf;
    sf::Music cooker;
    sf::Music wind;

    cooker.openFromFile("./models/audio/music.ogg");
    wind.openFromFile("./models/audio/wind.ogg");
    sound_buf.loadFromFile("./models/audio/steps.wav");
    //sound of cooking in RV
    sf::Listener::setPosition(-4, 1.5f, 5.f);
    sf::Listener::setDirection({ ModelPosition.x, ModelPosition .y, ModelPosition.z});
    cooker.setPosition(rv.position.x, rv.position.y, rv.position.z);
    cooker.setPitch(1.f);
    cooker.setVolume(80);
    cooker.setLoop(true);
    cooker.setMinDistance(1);
    cooker.setAttenuation(10);
    cooker.play();
    //sound of wind
    wind.setPosition(rv.position.x, rv.position.y, rv.position.z);
    wind.setPitch(1.f);
    wind.setVolume(90);
    wind.setLoop(true);
    wind.setMinDistance(100);
    wind.setAttenuation(0);
    wind.play();
    //sound of steps
    steps.setPosition(-4, 1.5f, 5.f);
    steps.setPitch(1.f);
    steps.setVolume(80);
    steps.setBuffer(sound_buf);
    steps.setLoop(false);
    steps.setMinDistance(3.f);
    steps.setAttenuation(0);
    steps.setRelativeToListener(true);

    //matricies
    glm::mat4 MMatr = WorldMatrix;
    glm::mat4 VMatr = WorldMatrix;
    glm::mat4 PMatr = WorldMatrix;

    PMatr = glm::perspective(
        glm::radians(80.f), // Угол обзора
        (float)WinWidth / WinHeight,       // Соотношение сторон
        0.1f,              // Ближняя плоскость отсечения 
        30.0f             // Дальняя плоскость отсечения
    );
    VMatr = cam.GetViewMatrix();
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    float rotation = 0;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /// Loop ///
    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, WinWidth, WinHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //camera mooved?
        if (cam.isMooved)
        {
            VMatr = WorldMatrix;
            VMatr = cam.GetViewMatrix();
            cam.isMooved = false;
            if (cam.isDisplaced) // чтобы не включалась от вращения головы
            {
                if (steps.getStatus() != sf::SoundSource::Playing) // prevent restarting
                    steps.play();
                cam.isDisplaced = false;
            }
        }
        //RV
        MMatr = rv.GetMMatrix({0.7f, 0.7f, 0.7f});
        rv.Draw(&MMatr[0][0], &VMatr[0][0], &PMatr[0][0]);
        //floor (ground)
        MMatr = floor.GetMMatrix();
        floor.Draw(&MMatr[0][0], &VMatr[0][0], &PMatr[0][0]);
        //brunches (3)
        brunch1.position = ModelPosition; //mooving by user
        brunch1.degree = ModelDegree;
        MMatr = brunch1.GetMMatrix();
        brunch1.Draw(&MMatr[0][0], &VMatr[0][0], &PMatr[0][0]);
        MMatr = brunch2.GetMMatrix();
        brunch2.Draw(&MMatr[0][0], &VMatr[0][0], &PMatr[0][0]);
        MMatr = brunch3.GetMMatrix();
        brunch3.Draw(&MMatr[0][0], &VMatr[0][0], &PMatr[0][0]);
        //chairs (2)
        MMatr = chair1.GetMMatrix({ 0.7f, 0.7f, 0.7f });
        chair1.Draw(&MMatr[0][0], &VMatr[0][0], &PMatr[0][0]);
        MMatr = chair2.GetMMatrix({ 0.7f, 0.7f, 0.7f });
        chair2.Draw(&MMatr[0][0], &VMatr[0][0], &PMatr[0][0]);
        //skybox
        MMatr = sky.GetMMatrix({2,2,2});
        glm::mat4 SkyVMatr = glm::mat4(glm::mat3(VMatr)); //no translate, scale and rotate only!
        sky.Draw(&MMatr[0][0], &SkyVMatr[0][0], &PMatr[0][0]);

        glfwPollEvents();  // Обработка событий GLFW
        glfwSwapBuffers(window);  // Обновление экрана
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))  // Обработка нажатия клавиши Escape
        {
            glfwSetWindowShouldClose(window, 1);  // Завершение программы
        }
    }
    return 0;
}

