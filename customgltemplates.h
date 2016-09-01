#ifndef CUSTOMGLTEMPLATES_H
#define CUSTOMGLTEMPLATES_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <string>

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoords;
};

#endif // CUSTOMGLTEMPLATES_H
