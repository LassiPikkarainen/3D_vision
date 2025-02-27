#include <iostream>
#include <thread>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <glad/glad.h>
#include<iostream>
#include<sstream>
#include<fstream>

#include <GLFW/glfw3.h>
//#include "SerialWriter.hpp"
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "common/loadShaders.hpp"
#include "window.hpp"

Window::Window() 
{
    //setup the window
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
}

int Window::Init(int width, int height) 
{
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    w = glfwCreateWindow(width, height, "3D", NULL, NULL);
    if (w == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);
    glfwMakeContextCurrent(w); // Initialize GLEW
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    printf("created window\n");

    
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    //set something here to initialize views
    view = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    UpdateViews(view);
    
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    programID = LoadShaders( "common/vertexShader.hpp", "common/fragmentShader.hpp" );

    model = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);

    mvp_left = projection * view_left * model; 
    mvp_right = projection * view_right * model; 


    // Only during the initialisation
    matrixID = glGetUniformLocation(programID, "MVP");


    //glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp_left[0][0]);

    //depth computation
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}


//generate view matrices for bot eyes TODO
void Window::UpdateViews(glm::mat4 view_to_use) 
{
    view_left = glm::lookAt(
                glm::vec3(3.9,3,4.1), // Camera is at (4,3,3), in World Space
                glm::vec3(0,0,0.1), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    
    view_right = glm::lookAt(
                glm::vec3(4.1,3,3.9), // Camera is at (4,3,3), in World Space
                glm::vec3(0.1,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
}


void Window::Run() 
{
    int i = 0; //even -> left frame, odd -> right frame
    while (!glfwWindowShouldClose(w)) 
    {
        //set the model view matrix thing to be used for the next frame
        if  (i % 2 == 0)
            glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp_left[0][0]);
        else
            glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp_right[0][0]);

        //clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        //vertex buffer
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        //color buffer
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, 12*3);// Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(w);

        i++; //increment frame counter

        glfwPollEvents(); //poll for events
    }
}