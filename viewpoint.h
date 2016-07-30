#ifndef VIEWPOINT_H
#define VIEWPOINT_H

#include "shader.h"
#include "model.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

GLfloat PI = glm::radians(180.f);

class viewPoint{
public:
    viewPoint(Shader* _shader, GLuint _width, GLuint _height){
        this->shader = _shader;
        this->clearColor=glm::vec4(1,1,1,1);
        this->position=glm::vec3(0,0,0);
        this->cameraDistance = 1;
        this->width = _width;
        this->height = _height;

        this->projection = glm::perspective(glm::radians(45.f),(GLfloat)width/height,0.1f, 100.f);

        //this->projection = glm::mat4();

        this->viewLocation = glGetUniformLocation(shader->Program,"view");
        this->projectionLocation = glGetUniformLocation(shader->Program,"projection");
    }
    void SetCamera(glm::vec3 position, glm::vec3 eulers){
        this->position = glm::vec3(position);
        this->eulers = glm::vec3(eulers);
        this->refreshMatrixes();
    }

    void UseCamera(){
        this->shader->Use();
        glUniformMatrix4fv(this->viewLocation,1,GL_FALSE,glm::value_ptr(this->view));
        glUniformMatrix4fv(this->projectionLocation,1,GL_FALSE,glm::value_ptr(this->projection));
        glClearColor(this->clearColor.x,this->clearColor.y,this->clearColor.z,this->clearColor.w);
    }

    void drawModel(model* target){
        target->DrawModel(this->shader);
    }

    void move(glm::vec3 direction){
        this->position += direction;
        this->refreshMatrixes();
    }

    void rotate(glm::vec3 _eulers){
        this->eulers+=_eulers;
        this->refreshMatrixes();
    }

    glm::vec3 getPosition(){
        return position;
    }

    glm::vec3 getEulers(){
        return eulers;
    }

private:
    Shader* shader;
    GLuint viewLocation;
    GLuint projectionLocation;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec4 clearColor;
    glm::vec3 position;
    glm::vec3 eulers;
    GLfloat cameraDistance;
    GLuint width,height;
    void refreshMatrixes(){
        while(eulers.x>2*PI)eulers.x-=2*PI;
        while(eulers.x<0)eulers.x+=2*PI;

        if(eulers.y>PI/2-0.05)eulers.y=PI/2-0.05;
        if(eulers.y<-PI/2+0.05)eulers.y=-PI/2+0.05;

        while(eulers.z>2*PI)eulers.z-=2*PI;
        while(eulers.z<0)eulers.z+=2*PI;

        glm::vec3 cameraTarget(-cameraDistance*glm::sin(eulers.x)*glm::cos(eulers.y),
                               cameraDistance*glm::sin(eulers.y),
                               cameraDistance*glm::cos(eulers.x)*glm::cos(eulers.y));
        this->view = glm::lookAt(this->position,this->position+cameraTarget,glm::vec3(0,1,0));
    }
};

#endif // VIEWPOINT_H
