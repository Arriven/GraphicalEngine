

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Other includes
#include "shader.h"
#include "viewpoint.h"
#include "model.h"
#include "interface.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint WIDTH = 800, HEIGHT = 600;

Shader* mainShader;
viewPoint* mainCam;
const GLuint modelsCount = 1;
model* mainModel;
model* secondModel;

GLFWwindow* window;

void initialize(){
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
    window = initializeInterface(WIDTH,HEIGHT);
    initialize();
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        doMovement(mainCam);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //mainCam->SetCamera(glm::vec3(0,0,glm::sin(glfwGetTime())),glm::vec3(0,0,0));

        //game logic
        secondModel->setPosition(glm::vec3(2,0,3),glm::vec3(0,glm::radians(90.f)-glfwGetTime(),0));
        mainModel->setPosition(glm::vec3(-2,0,3),glm::vec3(glfwGetTime(),0,0));

        mainCam->UseCamera();

        //drawing
        mainCam->drawModel(mainModel);
        mainCam->drawModel(secondModel);

        glfwSwapBuffers(window);
    }
    delete mainModel;
    glfwTerminate();
    return 0;
}
