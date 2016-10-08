#ifndef INTERFACE_H
#define INTERFACE_H

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Other includes
#include "shader.h"
#include "viewpoint.h"
#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);



void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void doMovement(viewPoint* camera);



GLFWwindow* initializeInterface(GLuint width, GLuint height);

#endif // INTERFACE_H
