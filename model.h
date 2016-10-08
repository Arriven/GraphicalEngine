#ifndef MODEL_H
#define MODEL_H


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "shader.h"
#include "customgltemplates.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <FreeImage.h>

GLuint ReadTexture(const char* path, std::string directory);

class mesh{
public:
    void Draw(Shader* shader);

    mesh(aiMesh* aiMesh, const aiScene* scene, std::string directory);

private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<GLuint> diffuse_textures;
    std::vector<GLuint> specular_textures;
    GLuint VAO,VBO,EBO;
    void setupMesh();
    std::vector<GLuint> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string directory);
    void loadMesh(aiMesh* aiMesh, const aiScene* scene, std::string directory);
};

class model{
public:
    glm::mat4 _model;
    model(std::string path);
    void setPosition(glm::vec3 position, glm::vec3 eulers);
    void DrawModel(Shader* shader);

private:
    std::vector<mesh> meshes;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
};

GLuint ReadTexture(const char* path, std::string directory);

#endif // MODEL_H
