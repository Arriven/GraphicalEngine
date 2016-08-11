#ifndef MODEL_H
#define MODEL_H


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "shader.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <FreeImage.h>

class model{
public:
    glm::mat4 _model;
    model(const GLchar* pathToModel, const GLchar* pathToTexture){
        std::cout<<"start reading model"<<std::endl;
        std::vector<float>vert;
        std::vector<float>textureCoords;
        std::vector<float>normals;
        std::vector<float>faces;
        std::cout<<faces.size()<<std::endl;
        std::ifstream in;
        in.exceptions(std::ifstream::badbit);
        in.open(pathToModel,std::ifstream::in);
        if(in.fail())return;
        std::cout<<"file read"<<std::endl;
        std::string line;
        while(!in.eof()){
            std::getline(in, line);
            std::istringstream iss(line.c_str());
            char trash;
            if(!line.compare(0,2,"v ")){
                iss>>trash;
                float val;
                for(int i = 0;i<3;i++){iss>>val;vert.push_back(val);}
            }
            else if(!line.compare(0,3,"vn ")){
                iss>>trash>>trash;
                float val;
                for(int i = 0;i<3;i++){iss>>val;normals.push_back(val);}
            }
            else if(!line.compare(0,3,"vt ")){
                iss>>trash>>trash;               
                float val;
                for(int i = 0;i<2;i++){iss>>val;textureCoords.push_back(val);}
            }
            else if(!line.compare(0,2,"f ")){
                iss>>trash;
                glm::ivec3 indexes;
                for(int i = 0;i<3;i++){
                    iss>>indexes.x>>trash>>indexes.y>>trash>>indexes.z;
                    for(int j=0;j<3;j++)
                        faces.push_back(vert[(indexes.x-1)*3+j]);
                    for(int j = 0; j < 2; j++)
                        faces.push_back(textureCoords[(indexes.y-1)*2+j]);
                }
            }
        }
        generateData(faces);



        BindTexture(pathToTexture);
    }
    ~model(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
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
        glBindTexture(GL_TEXTURE_2D, this->texture);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0, this->nVertices);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    GLuint nVertices;
    GLuint VAO,VBO;
    GLuint texture;
    void BindTexture(const GLchar* pathToTexture){
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height;

        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(pathToTexture,0);
        FIBITMAP* image = FreeImage_Load(format, pathToTexture);

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
    }
    void generateData(std::vector<float> vertices){
        this->nVertices = vertices.size()/5;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GLfloat),&vertices[0],GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

#endif // MODEL_H
