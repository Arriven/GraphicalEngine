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
    void Draw(Shader* shader){
        GLuint textureCounter = 0;
        for(GLuint i = 0;i < this->diffuse_textures.size();i++){
            glActiveTexture(GL_TEXTURE0+textureCounter);
            std::stringstream ss;
            ss<<i;
            std::string type = "texture_diffuse";
            glUniform1f(glGetUniformLocation(shader->Program, ("material." + type + ss.str()).c_str()), textureCounter);
            glBindTexture(GL_TEXTURE_2D,this->diffuse_textures[i]);
            textureCounter++;
        }
        for(GLuint i = 0;i < this->specular_textures.size();i++){
            glActiveTexture(GL_TEXTURE0+textureCounter);
            std::stringstream ss;
            ss<<i;
            std::string type = "texture_specular";
            glUniform1f(glGetUniformLocation(shader->Program, ("material." + type + ss.str()).c_str()), textureCounter);
            glBindTexture(GL_TEXTURE_2D,this->specular_textures[i]);
            textureCounter++;
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    mesh(aiMesh* aiMesh, const aiScene* scene, std::string directory){
        this->loadMesh(aiMesh, scene, directory);
    }

private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<GLuint> diffuse_textures;
    std::vector<GLuint> specular_textures;
    GLuint VAO,VBO,EBO;
    void setupMesh()
    {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                     &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                     &this->indices[0], GL_STATIC_DRAW);

        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             (GLvoid*)offsetof(Vertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             (GLvoid*)offsetof(Vertex, TextureCoords));

        glBindVertexArray(0);
    }
    std::vector<GLuint> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string directory)
    {
        std::vector<GLuint> textures;
        for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            GLuint id = ReadTexture(str.C_Str(), directory);
            textures.push_back(id);
        }
        return textures;
    }
    void loadMesh(aiMesh* aiMesh, const aiScene* scene, std::string directory){
        for(GLuint i = 0; i < aiMesh->mNumVertices; i++)
            {
                Vertex vertex;
                // Process vertex positions, normals and texture coordinates
                glm::vec3 vector;
                vector.x = aiMesh->mVertices[i].x;
                vector.y = aiMesh->mVertices[i].y;
                vector.z = aiMesh->mVertices[i].z;
                vertex.Position = vector;

                vector.x = aiMesh->mNormals[i].x;
                vector.y = aiMesh->mNormals[i].y;
                vector.z = aiMesh->mNormals[i].z;
                vertex.Normal = vector;

                if(aiMesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    vec.x = aiMesh->mTextureCoords[0][i].x;
                    vec.y = aiMesh->mTextureCoords[0][i].y;
                    vertex.TextureCoords = vec;
                }
                else
                    vertex.TextureCoords = glm::vec2(0.0f, 0.0f);


                this->vertices.push_back(vertex);
                this->setupMesh();
            }

        for(GLuint i = 0; i < aiMesh->mNumFaces; i++)
        {
            aiFace face = aiMesh->mFaces[i];
            for(GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        if(aiMesh->mMaterialIndex >= 0)
            {
                aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
                this->diffuse_textures=loadMaterialTextures(material,aiTextureType_DIFFUSE,directory);
                this->specular_textures=loadMaterialTextures(material,aiTextureType_SPECULAR,directory);
            }
    }
};

class model{
public:
    glm::mat4 _model;
    model(GLchar* path){
        this->loadModel(path);
    }
    void setPosition(glm::vec3 position, glm::vec3 eulers){
        this->_model = glm::mat4();
        this->_model = glm::translate(this->_model,position);
        this->_model = glm::rotate(this->_model,eulers.x,glm::vec3(1,0,0));
        this->_model = glm::rotate(this->_model,eulers.y,glm::vec3(0,1,0));
        this->_model = glm::rotate(this->_model,eulers.z,glm::vec3(0,0,1));
    }
    void DrawModel(Shader* shader){
        GLuint modelLocation = glGetUniformLocation(shader->Program,"model");
        glUniformMatrix4fv(modelLocation,1,GL_FALSE,glm::value_ptr(this->_model));
        for(GLuint i = 0; i < this->meshes.size(); i++)
                this->meshes[i].Draw(shader);
    }

private:
    std::vector<mesh> meshes;
    std::string directory;
    void loadModel(std::string path){
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        this->directory = path.substr(0, path.find_last_of('/'));

        this->processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode* node, const aiScene* scene)
    {
        // Process all the node's meshes (if any)
        for(GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(mesh(aimesh, scene,this->directory));
        }
        // Then do the same for each of its children
        for(GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }
    }
};

GLuint ReadTexture(const char* path, std::string directory){
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(),0);
    FIBITMAP* image = FreeImage_Load(format, filename.c_str());

    FIBITMAP* temp = image;
    image = FreeImage_ConvertTo32Bits(image);
    FreeImage_Unload(temp);

    width = FreeImage_GetWidth(image);
    height = FreeImage_GetHeight(image);

    GLubyte* textureBytes = new GLubyte[4*width*height];
    char* pixels = (char*)FreeImage_GetBits(image);


    for(int j= 0; j<width*height; j++){
        textureBytes[j*4+0]= pixels[j*4+2];
        textureBytes[j*4+1]= pixels[j*4+1];
        textureBytes[j*4+2]= pixels[j*4+0];
        textureBytes[j*4+3]= pixels[j*4+3];
        //cout<<j<<": "<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)textureBytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

#endif // MODEL_H
