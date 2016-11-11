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
    //main Draw function
    void Draw(Shader* shader);
    //constructor
    mesh(aiMesh* aiMesh, const aiScene* scene, std::string directory);
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<GLuint> diffuse_textures;
    std::vector<GLuint> specular_textures;
    GLuint VAO,VBO,EBO;
    //move all the data into the buffers
    void setupMesh();
    //method, that allows to load textures
    std::vector<GLuint> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string directory);
    //prepare the data for the mesh
    void loadMesh(aiMesh* aiMesh, const aiScene* scene, std::string directory);
};

class model{
public:
    model(std::string path);
    const glm::vec3 getPosition() const;
    const glm::vec3 getEulers() const;
    void setPosition(glm::vec3 position, glm::vec3 eulers);
    void move(glm::vec3 direction);
    void rotate(glm::vec3 direction);
    void DrawModel(Shader* shader);
private:
    glm::mat4 _model;
    glm::vec3 _position;
    glm::vec3 _eulers;
    std::vector<mesh> meshes;
    std::string directory;
    void RefreshMatrix();
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
};

//Read texture from file
GLuint ReadTexture(const char* path, std::string directory);

#endif // MODEL_H
