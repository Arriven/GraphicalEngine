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



class viewPoint{
public:
    viewPoint(Shader* _shader, GLuint _width, GLuint _height);
    void SetCamera(glm::vec3 position, glm::vec3 eulers);
    void UseCamera();
    void drawModel(model* target);
    void move(glm::vec3 direction);
    void rotate(glm::vec3 _eulers);
    glm::vec3 getPosition();
    glm::vec3 getEulers();
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

    void refreshMatrixes();
};

#endif // VIEWPOINT_H
