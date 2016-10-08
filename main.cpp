

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
model* nanosuit;

GLFWwindow* window;

void initialize(){
    nanosuit = new model("nanosuit/nanosuit.obj");

    mainShader = new Shader("vertex-shader.vs",
                        "fragment-shader.frag");

    mainCam = new viewPoint(mainShader,WIDTH,HEIGHT);
    mainCam->SetCamera(glm::vec3(0,0,-3),glm::vec3(0,0,0));
}

int main(){
    window = initializeInterface(WIDTH,HEIGHT);
    initialize();
    while(!glfwWindowShouldClose(window)){
        //std::cout<<"frame"<<std::endl;
        glfwPollEvents();
        doMovement(mainCam);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //mainCam->SetCamera(glm::vec3(0,0,glm::sin(glfwGetTime())),glm::vec3(0,0,0));

        //game logic
        nanosuit->setPosition(glm::vec3(0,0,0),glm::vec3(0,0,0));

        mainCam->UseCamera();

        //drawing
        mainCam->drawModel(nanosuit);
        glfwSwapBuffers(window);
    }
    delete nanosuit;
    glfwTerminate();
    return 0;
}
