#include "viewpoint.h"

GLfloat PI = glm::radians(180.f);

viewPoint::viewPoint(Shader* _shader, GLuint _width, GLuint _height){
    this->shader = _shader;
    this->clearColor=glm::vec4(0.5f,0.5f,0.5f,1);
    this->position=glm::vec3(0,0,0);
    this->cameraDistance = 1;
    this->width = _width;
    this->height = _height;

    this->projection = glm::perspective(glm::radians(45.f),(GLfloat)width/height,0.1f, 100.f);

    //this->projection = glm::mat4();

    this->viewLocation = glGetUniformLocation(shader->Program,"view");
    this->projectionLocation = glGetUniformLocation(shader->Program,"projection");
}
void viewPoint::SetCamera(glm::vec3 position, glm::vec3 eulers){
    this->position = glm::vec3(position);
    this->eulers = glm::vec3(eulers);
    this->refreshMatrixes();
}

void viewPoint::UseCamera(){
    this->shader->Use();
    glUniformMatrix4fv(this->viewLocation,1,GL_FALSE,glm::value_ptr(this->view));
    glUniformMatrix4fv(this->projectionLocation,1,GL_FALSE,glm::value_ptr(this->projection));
    glClearColor(this->clearColor.x,this->clearColor.y,this->clearColor.z,this->clearColor.w);
}

void viewPoint::drawModel(model* target){
    target->DrawModel(this->shader);
}

void viewPoint::move(glm::vec3 direction){
    this->position += direction;
    this->refreshMatrixes();
}

void viewPoint::rotate(glm::vec3 _eulers){
    this->eulers+=_eulers;
    this->refreshMatrixes();
}

glm::vec3 viewPoint::getPosition(){
    return position;
}

glm::vec3 viewPoint::getEulers(){
    return eulers;
}

void viewPoint::refreshMatrixes(){
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
