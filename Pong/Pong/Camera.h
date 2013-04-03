#ifndef Pong_Camera_h
#define Pong_Camera_h
#include "Common.h"

class Camera{
public:
    Camera(){
        projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
        viewMatrix = glm::mat4(1.0f);
    };
    void translate(GLfloat x, GLfloat y, GLfloat z){
        viewMatrix = glm::translate(viewMatrix, x, y, z);
    }
    void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z){
        viewMatrix = glm::rotate(viewMatrix, angle, x, y, z);
    }
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};


#endif