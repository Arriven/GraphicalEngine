

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include<SOIL/SOIL.h>

// Other includes
#include "shader.h"
#include "viewpoint.h"
#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint WIDTH = 800, HEIGHT = 600;

Shader* mainShader;
viewPoint* mainCam;
const GLuint modelsCount = 1;
model* mainModel;
model* secondModel;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLFWwindow* window;

void initialize(){

    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    //models[0]=model("path/to/model","");
    //mainCam = &viewPoint(&Shader("",""),WIDTH,HEIGHT);
    glEnable(GL_DEPTH_TEST);


    mainModel = new model("/home/arriven/projects/build-GraphicalEngine-Desktop-Debug/african_head.obj",
                      "/home/arriven/projects/build-GraphicalEngine-Desktop-Debug/african_head_diffuse.tga");

    secondModel = new model("/home/arriven/projects/build-GraphicalEngine-Desktop-Debug/african_head.obj",
                            "/home/arriven/projects/build-GraphicalEngine-Desktop-Debug/african_head_diffuse.tga");

    mainShader = new Shader("/home/arriven/projects/build-GraphicalEngine-Desktop-Debug/vertex-shader.vs",
                        "/home/arriven/projects/build-GraphicalEngine-Desktop-Debug/fragment-shader.frag");

    mainCam = new viewPoint(mainShader,WIDTH,HEIGHT);
    mainCam->SetCamera(glm::vec3(0,0,-3),glm::vec3(0,0,0));
}

int main(){
    initialize();
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //mainCam->SetCamera(glm::vec3(0,0,glm::sin(glfwGetTime())),glm::vec3(0,0,0));

        secondModel->setPosition(glm::vec3(2,0,3),glm::vec3(0,glm::radians(90.f)-glfwGetTime(),0));
        mainModel->setPosition(glm::vec3(-2,0,3),glm::vec3(glfwGetTime(),0,0));
        mainCam->UseCamera();

        mainCam->drawModel(mainModel);
        mainCam->drawModel(secondModel);
        glfwSwapBuffers(window);
    }
    delete mainModel;
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
