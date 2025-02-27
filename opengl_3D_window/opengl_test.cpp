//#include <processthreadsapi.h>
#include <iostream>
//#include <minwindef.h>
#include <thread>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SerialWriter.hpp"
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "common/loadShaders.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

extern bool send_flag = false;
extern char side_to_show;
extern int delaymicros = 100; // allows for delay to be manually configured

char curFrame = 'L';
char delay_command = 'S';
char increase_period_command = 'M';
char decrease_period_command = 'N';

char readbuffer[20];

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool right_pressed = false;
bool up_pressed = false;
bool down_pressed = false;


int main()
{
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    //setup serial comms
    char byte[4] = {'a'}; //some char to send 
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    //LPCSTR wComport = "\\\\.\\COM3";
    LPCSTR wComport = "COM3";
    printf("Opening serial port...");
    hSerial = CreateFile(
                wComport, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if (hSerial == INVALID_HANDLE_VALUE)
    {
            printf("Error\n");
    }
    else printf("OK\n");

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        printf("Error getting device state\n");
        CloseHandle(hSerial);
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        printf("Error setting device parameters\n");
        CloseHandle(hSerial);
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if(SetCommTimeouts(hSerial, &timeouts) == 0)
    {
        printf("Error setting timeouts\n");
        CloseHandle(hSerial);
    }
    printf("Done");
    DWORD bytes_written, total_bytes_written = 0;

    DWORD bytes_read;



    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    //std::thread sendthread(SerialWriter);
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);
    glfwMakeContextCurrent(window); // Initialize GLEW
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    printf("created window\n");


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    static const GLfloat g_vertex_buffer_data[] = {
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

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    static const GLfloat g_color_buffer_data[] = {
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

    

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    GLuint programID = LoadShaders( "common/vertexShader.hpp", "common/fragmentShader.hpp" );

    auto start_time = std::chrono::high_resolution_clock::now();


    //coordinate transform matrices
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    
    // Model matrix: an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection: multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    //depth computation
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    send_flag = true; //set flag for the send thread

    int i = 1;

    while (!glfwWindowShouldClose(window))
    //for (int i = 0; i < 1000; i++)
    {

        //change view matrix based on right / left frame

        if (curFrame == 'L') 
        {
             View = glm::lookAt(
                glm::vec3(3.9,3,4.1), // Camera is at (4,3,3), in World Space
                glm::vec3(0,0,0.1), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
            );
        }
        else if (curFrame == 'R' ) 
        {
            View = glm::lookAt(
                glm::vec3(4.1,3,3.9), // Camera is at (4,3,3), in World Space
                glm::vec3(0.1,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
            );
        }
        /*
        if(!WriteFile(hSerial, &curFrame, 1, &bytes_written, NULL))
        {
            printf("Error\n");
            CloseHandle(hSerial);
            return 1;
        }
*/


        glm::mat4 mvp = Projection * View * Model; 
        MatrixID = glGetUniformLocation(programID, "MVP");
        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        
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


        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3);// Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        //Sleep(1);
        if (curFrame == 'L') {
            curFrame = 'R';
        }
        else if (curFrame == 'R') {
            curFrame = 'L';
        }
        side_to_show = curFrame;

        //WriteFile(hSerial, &side_to_show, 1, &bytes_written, NULL);

        glfwPollEvents();
        //right key
        if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS && !right_pressed){
            delay = true;
            right_pressed = true;
            printf("right arrow key pressed\n");
            WriteFile(hSerial, &delay_command, 1, &bytes_written, NULL);
        } 
        //up
        if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS && !up_pressed){
            up_pressed = true;
            printf("up arrow key pressed\n");
            WriteFile(hSerial, &increase_period_command, 1, &bytes_written, NULL);
        } 
            //up
        if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS && !down_pressed){
            down_pressed = true;
            printf("down arrow key pressed\n");
            WriteFile(hSerial, &decrease_period_command, 1, &bytes_written, NULL);
        } 

        //flag resets
        if (glfwGetKey( window, GLFW_KEY_RIGHT ) != GLFW_PRESS) 
        {
            right_pressed = false;
        }

        if (glfwGetKey( window, GLFW_KEY_UP ) != GLFW_PRESS) 
        {
            up_pressed = false;
        }
        if (glfwGetKey( window, GLFW_KEY_DOWN ) != GLFW_PRESS) 
        {
            down_pressed = false;
        }
        /*
        ReadFile(hSerial, &readbuffer, 20, &bytes_read, NULL);
        if (bytes_read > 0) {
            printf("%s\n",readbuffer);
        }
        */
        i++;

        //time taken by n loops 
        if (i == 1000) 
        {
            auto elapsed = std::chrono::high_resolution_clock::now() - start_time;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            printf("%lld\n", microseconds/1000);
            start_time = std::chrono::high_resolution_clock::now();
            i = 1;
        }
        

        
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - start_time;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    double millis = microseconds / 1000 / 1000;
    printf("%f\n", millis);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !right_pressed)
        glfwSetWindowShouldClose(window, true);
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



