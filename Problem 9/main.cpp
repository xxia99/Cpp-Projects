/*
Author: Xin Xia
Class: ECE 6122
Last Date Modified: 12/4/2021
Description: This is a program simulating a half-time show using UAVs developed with std::thread and OpenGL.
Reference: https://github.com/opengl-tutorials/controller, some codes & comments are fetched from this open-source project.
*/
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <eigen3/Eigen/StdVector>
#include "ECE_UAV.h"

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/controls.hpp"
#include "../common/objloader.hpp"
#include "../common/texture.hpp"
#include "../common/shader.hpp"
#include "../common/vboindexer.hpp"

using namespace std;
using namespace chrono;
using namespace glm;

vector<Vector3f*> uavGroup;
GLFWwindow* glfWindow;
glm::vec3 uavOrientation(1.57, 0, 0);

int controlOpenGL()
{
    // Initialize the main GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    glfWindow = glfwCreateWindow(400, 400, "GaTech Buzzy Bowl", NULL, NULL);
    if( glfWindow == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(glfWindow);// Set window context as current thread context

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(glfWindow, GLFW_STICKY_KEYS, GL_TRUE);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(glfWindow, 200, 200);  // Set the cursor in the middle of the window

    // Initialize the background to white
    glClearColor(0.99f, 0.99f, 0.99f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LEFT);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE_MODE);

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Create and compile our GLSL program from the UAV shaders
    GLuint uavShader = LoadShaders( "uavVertexShader.vertexshader","uavFragmentShader.fragmentshader" );
    // Create and compile our GLSL program from the Football Field shaders
    GLuint fieldShader = LoadShaders( "fieldVertexShader.vertexshader","fieldFragmentShader.fragmentshader" );
    // Create and compile our GLSL program from the Sphere shaders
    GLuint sphereShader = LoadShaders( "sphereVertexShader.vertexshader","sphereFragmentShader.fragmentshader" );

    // Get a handle for "MVP" uniform for UAV
    GLuint uavMatrixID = glGetUniformLocation(uavShader, "uavMVP");
    // Get a handle for "MVP" uniform for football field
    GLuint fieldMatrixID = glGetUniformLocation(fieldShader, "fieldMVP");
    // Get a handle for "MVP" uniform for virtual sphere
    GLuint sphereMatrixID = glGetUniformLocation(sphereShader, "sphereMVP");

    // Load the texture for UAV and football field
    GLuint uavIMG = loadBMP_custom("uav.bmp");
    GLuint fieldIMG = loadBMP_custom("ff.bmp");

    // Get a handle for our "uavTexture" uniform
    GLuint uavTextureID  = glGetUniformLocation(uavShader, "uavTexture");
    // Get a handle for our "fieldTexture" uniform
    GLuint fieldTextureID  = glGetUniformLocation(fieldShader, "fieldTexture");

    // Set the texture wrapping parameters and texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Read the .obj files
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> spVector;
    loadOBJ("../OBJ files/shipA_OBJ/ship2.obj", vertices, uvs, normals);
    loadNoUVOBJ("../OBJ files/sphere.obj", spVector);

    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);

    GLuint vnBuffer;
    glGenBuffers(1, &vnBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vnBuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices
    GLuint elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Generate a buffer for the sphere vectors
    GLuint spBuffer;
    glGenBuffers(1, &spBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, spBuffer);
    glBufferData(GL_ARRAY_BUFFER, spVector.size() * sizeof(glm::vec3), &spVector[0], GL_STATIC_DRAW);

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A football field (rectangle) has 2 triangles each, so this makes 2*3=6 vertices
    static const GLfloat g_vertex_buffer_data[] =
            {
                    -1.f, 0.5f, 0.0f,  // upper left
                    -1.f,-0.5f, 0.0f,  // lower left
                    1.f, -0.5f, 0.0f,  // lower right
                    1.f, -0.5f, 0.0f,  // lower right
                    1.f,  0.5f, 0.0f,  // upper right
                    -1.f, 0.5f, 0.0f,  // upper left

            };
    // Two UV coordinates for each vertex. They were created with Blender.
    static const  GLfloat g_uv_buffer_data[] =
            {
                    0.07f, 1.f,  // upper left
                    0.07f, 0.f,  // lower left
                    1.0f,0.f,    // lower right
                    1.0f,0.f,    // lower right
                    1.f, 1.f,    // upper right
                    0.07f, 1.f,  // upper left
            };

    GLuint vboBuffer;
    glGenBuffers(1, &vboBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vboBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint uvsBuffer;
    glGenBuffers(1, &uvsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    do
    {
        // Clear screen before a new render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Use the shader for football field
        glUseProgram(fieldShader);
        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0), glm::vec3(65.0, 63.0, 1.0));
        glm::mat4 ModelMatrix = ScalingMatrix;
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 fieldMVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,in the "MVP" uniform
        glUniformMatrix4fv(fieldMatrixID, 1, GL_FALSE, &fieldMVP[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fieldIMG);  // Bind our football field texture in texture Unit 0
        glUniform1i(fieldTextureID, 0);  // Set "ffTexture" sampler to use Texture Unit 0

        // 1st attribute buffer : vertices
        glBindBuffer(GL_ARRAY_BUFFER, vboBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,                  // attribute no.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // 2nd attribute buffer
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);
        glVertexAttribPointer(
                1,                  // attribute no.
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // Draw the triangle for the field
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Draw the UAVs in the following step
        glDisable(GL_BLEND);
        // Use shader for UAVs
        glUseProgram(uavShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, uavIMG);  // Bind the UAV flying texture in texture Unit 0
        glUniform1i(uavTextureID, 0);  // Set "UAVTexture" sampler to use Texture Unit 0

        for (auto uav:uavGroup)
        {
            glm::vec3 gPosition((*uav)[1],(*uav)[0],(*uav)[2]);

            // Compute the MVP matrix from keyboard and mouse input
            computeMatricesFromInputs();
            glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.f), uavOrientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0), gPosition);
            glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.07f, 0.07f, 0.07f));
            glm::mat4 ModelMatrix =  TranslationMatrix * RotationMatrix * ScalingMatrix;
            glm::mat4 ViewMatrix = getViewMatrix();
            glm::mat4 ProjectionMatrix = getProjectionMatrix();
            glm::mat4 uavMVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            // Send the transformation to the currently bound shader in the "MVP" uniform
            glUniformMatrix4fv(uavMatrixID, 1, GL_FALSE, &uavMVP[0][0]);
            glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
            glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
            );

            // 2nd attribute buffer : UVs
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
            glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
            );

            // Draw the UAV flying above the football field
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, vnBuffer);
        glVertexAttribPointer(
                2,                       // attribute
                3,                       // size
                GL_FLOAT,                // type
                GL_FALSE,                // normalized
                0,                       // stride
                (void*)0                 // array buffer offset
        );
        glBindVertexArray(vertexArrayID);

        // Draw the UAV flying above the football field
        glDrawElements(
                GL_TRIANGLES,      // mode
                indices.size(),    // count
                GL_UNSIGNED_SHORT, // type
                (void*)0   // element array buffer offset
        );

        // Draw the UAVs in the following step
        // Use the shader for sphere
        glUseProgram(sphereShader);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 50.f));
        glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
        glm::mat4 ModelMatrix = TranslationMatrix * ScalingMatrix;
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 sphereMVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,in the "MVP" uniform
        glUniformMatrix4fv(sphereMatrixID, 1, GL_FALSE, &sphereMVP[0][0]);

        // 1st attribute buffer : vertices
        glBindBuffer(GL_ARRAY_BUFFER, spBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,                  // attribute no.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized
                3 * sizeof(float),   // stride
                (void *) 0           // array buffer offset
        );
        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, spVector.size());
        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(glfWindow);
        glfwPollEvents();

        // The main thread polls the UAV once every 30 msec
        this_thread::sleep_for(chrono::milliseconds(30));
    }
    while (glfwGetKey(glfWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(glfWindow) == 0);
    // Check if the ESC key was pressed or the window was closed

    // Remove the shaders and clean up the buffers
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &vnBuffer);
    glDeleteBuffers(1, &elementBuffer);
    glDeleteBuffers(1, &spBuffer);
    glDeleteBuffers(1, &vboBuffer);
    glDeleteBuffers(1, &uvsBuffer);
    glDeleteProgram(sphereShader);
    glDeleteProgram(uavShader);
    glDeleteProgram(fieldShader);
    glDeleteTextures(1, &uavIMG);
    glDeleteTextures(1, &fieldIMG);
    glDeleteVertexArrays(1, &vertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

int main()
{
    thread controller(controlOpenGL);
    ECE_UAV pilot1(0, 0);
    ECE_UAV pilot2(0, 25);
    ECE_UAV pilot3(0, -25);
    ECE_UAV pilot4(0, 50);
    ECE_UAV pilot5(0, -50);
    ECE_UAV pilot6(25, 0);
    ECE_UAV pilot7(25, 25);
    ECE_UAV pilot8(25, -25);
    ECE_UAV pilot9(25, 50);
    ECE_UAV pilot10(25, -50);
    ECE_UAV pilot11(-25, 0);
    ECE_UAV pilot12(-25, 25);
    ECE_UAV pilot13(-25, -25);
    ECE_UAV pilot14(-25, 50);
    ECE_UAV pilot15(-25, -50);
    uavGroup.push_back(pilot1.findUAV());
    uavGroup.push_back(pilot2.findUAV());
    uavGroup.push_back(pilot3.findUAV());
    uavGroup.push_back(pilot4.findUAV());
    uavGroup.push_back(pilot5.findUAV());
    uavGroup.push_back(pilot6.findUAV());
    uavGroup.push_back(pilot7.findUAV());
    uavGroup.push_back(pilot8.findUAV());
    uavGroup.push_back(pilot9.findUAV());
    uavGroup.push_back(pilot10.findUAV());
    uavGroup.push_back(pilot11.findUAV());
    uavGroup.push_back(pilot12.findUAV());
    uavGroup.push_back(pilot13.findUAV());
    uavGroup.push_back(pilot14.findUAV());
    uavGroup.push_back(pilot15.findUAV());
    pilot1.start();
    pilot1.join();
    pilot2.start();
    pilot2.join();
    pilot3.start();
    pilot3.join();
    pilot4.start();
    pilot4.join();
    pilot5.start();
    pilot5.join();
    pilot6.start();
    pilot6.join();
    pilot7.start();
    pilot7.join();
    pilot8.start();
    pilot8.join();
    pilot9.start();
    pilot9.join();
    pilot10.start();
    pilot10.join();
    pilot11.start();
    pilot11.join();
    pilot12.start();
    pilot12.join();
    pilot13.start();
    pilot13.join();
    pilot14.start();
    pilot14.join();
    pilot15.start();
    pilot15.join();
    controller.join();
    return -1;
}


