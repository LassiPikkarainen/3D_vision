#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <thread>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<sstream>
#include<fstream>

//#include "SerialWriter.hpp"
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



class Window 
{
public:
    Window();
    ~Window();

    int Init(int width, int height);
    void Run();

    void UpdateViews(glm::mat4 view_to_use);



private:
    GLFWwindow* w;
    GLfloat g_vertex_buffer_data[108]  = {
        // Front face
        -1.0f, -1.0f,  1.0f,  
        1.0f, -1.0f,  1.0f,  
        1.0f,  1.0f,  1.0f,  
        -1.0f, -1.0f,  1.0f,  
        1.0f,  1.0f,  1.0f,  
        -1.0f,  1.0f,  1.0f,  

        // Back face
        -1.0f, -1.0f, -1.0f,  
        -1.0f,  1.0f, -1.0f,  
        1.0f,  1.0f, -1.0f,  
        -1.0f, -1.0f, -1.0f,  
        1.0f,  1.0f, -1.0f,  
        1.0f, -1.0f, -1.0f,  

        // Left face
        -1.0f, -1.0f, -1.0f,  
        -1.0f, -1.0f,  1.0f,  
        -1.0f,  1.0f,  1.0f,  
        -1.0f, -1.0f, -1.0f,  
        -1.0f,  1.0f,  1.0f,  
        -1.0f,  1.0f, -1.0f,  

        // Right face
        1.0f, -1.0f, -1.0f,  
        1.0f,  1.0f, -1.0f,  
        1.0f,  1.0f,  1.0f,  
        1.0f, -1.0f, -1.0f,  
        1.0f,  1.0f,  1.0f,  
        1.0f, -1.0f,  1.0f,  

        // Top face
        -1.0f,  1.0f, -1.0f,  
        -1.0f,  1.0f,  1.0f,  
        1.0f,  1.0f,  1.0f,  
        -1.0f,  1.0f, -1.0f,  
        1.0f,  1.0f,  1.0f,  
        1.0f,  1.0f, -1.0f,  

        // Bottom face
        -1.0f, -1.0f, -1.0f,  
        1.0f, -1.0f, -1.0f,  
        1.0f, -1.0f,  1.0f,  
        -1.0f, -1.0f, -1.0f,  
        1.0f, -1.0f,  1.0f,  
        -1.0f, -1.0f,  1.0f  
    };
    GLfloat g_color_buffer_data[108] = {
        // Front face (Red)
        1.0f, 0.0f, 0.0f,  
        1.0f, 0.0f, 0.0f,  
        1.0f, 0.0f, 0.0f,  
        1.0f, 0.0f, 0.0f,  
        1.0f, 0.0f, 0.0f,  
        1.0f, 0.0f, 0.0f,  

        // Back face (Green)
        0.0f, 1.0f, 0.0f,  
        0.0f, 1.0f, 0.0f,  
        0.0f, 1.0f, 0.0f,  
        0.0f, 1.0f, 0.0f,  
        0.0f, 1.0f, 0.0f,  
        0.0f, 1.0f, 0.0f,  

        // Left face (Blue)
        0.0f, 0.0f, 1.0f,  
        0.0f, 0.0f, 1.0f,  
        0.0f, 0.0f, 1.0f,  
        0.0f, 0.0f, 1.0f,  
        0.0f, 0.0f, 1.0f,  
        0.0f, 0.0f, 1.0f,  

        // Right face (Yellow)
        1.0f, 1.0f, 0.0f,  
        1.0f, 1.0f, 0.0f,  
        1.0f, 1.0f, 0.0f,  
        1.0f, 1.0f, 0.0f,  
        1.0f, 1.0f, 0.0f,  
        1.0f, 1.0f, 0.0f,  

        // Top face (Magenta)
        1.0f, 0.0f, 1.0f,  
        1.0f, 0.0f, 1.0f,  
        1.0f, 0.0f, 1.0f,  
        1.0f, 0.0f, 1.0f,  
        1.0f, 0.0f, 1.0f,  
        1.0f, 0.0f, 1.0f,  

        // Bottom face (Cyan)
        0.0f, 1.0f, 1.0f,  
        0.0f, 1.0f, 1.0f,  
        0.0f, 1.0f, 1.0f,  
        0.0f, 1.0f, 1.0f,  
        0.0f, 1.0f, 1.0f,  
        0.0f, 1.0f, 1.0f 
    };
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint programID;
    GLuint matrixID;
    glm::mat4 projection;
    glm::mat4 view; // "original view matrix"
    glm::mat4 view_left; // left eye view
    glm::mat4 view_right; // right eye view
    glm::mat4 model; //placement for model
    glm::mat4 mvp_left; // model view matrix (left and right eyes)
    glm::mat4 mvp_right;

};

#endif