//
//  main.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-24.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include <glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include "Loader.h"


//#include "Mesh.h"

using namespace std;

int main() {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    // uncomment these lines if on Apple OS X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    
    glViewport(0, 0, 1280, 720);
    
//    float points[] = {
//        0.0f,  0.5f,  0.0f,
//        0.5f, -0.5f,  0.0f,
//        -0.5f, -0.5f,  0.0f
//    };
//    
//    GLuint vbo = 0;
//    glGenBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);
//    
//    GLuint vao = 0;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
//    Mesh * mesh = new Mesh();
//    mesh->loadMesh("Pikachu.dae");
    
//    GLuint vao;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
    
    GLuint program = LoadShader("shaders/shader.vert", "shaders/shader.frag");
    
    glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, 10.0f);
    glm::vec3 lookAt = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1.0f, 0);
    
    glm::mat4 view = glm::lookAt(cameraPos, lookAt, up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.01f, 100.0f);
    
    glm::mat4 viewProjection = projection * view;
    glUseProgram(program);

    GLint mLoc = glGetUniformLocation(program, "mvpMatrix");
    glm::mat4 identity = glm::mat4();
    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));
    
//    std::cout<<glm::to_string(projection)<<std::endl;
    
    float vertices[] = {
        //RED
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        
        //ORANGE
        0.5f, 0.5f, -0.5f, 1.0f, 0.6f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.6f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.6f, 0.0f,
        
        //YELLOW
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        
        //ORANGE
        0.5f, 0.5f, -0.5f, 1.0f, 0.6f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.6f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.6f, 0.0f,
        
        //RED
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        
        //YELLOW
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        
        //GREEN
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        
        //BLUE
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        
        //BLUE
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        
        //INDIGO
        0.5f, 0.5f, 0.5f, 0.294f, 0.0f, 0.51f,
        0.5f, 0.5f, -0.5f, 0.294f, 0.0f, 0.51f,
        -0.5f, 0.5f, -0.5f, 0.294f, 0.0f, 0.51f,
        
        //INDIGO
        0.5f, 0.5f, 0.5f, 0.294f, 0.0f, 0.51f,
        -0.5f, 0.5f, -0.5f, 0.294f, 0.0f, 0.51f,
        -0.5f, 0.5f, 0.5f, 0.294f, 0.0f, 0.51f,
        
        //GREEN
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        
    };
    
//    float vertices[] = {
//        0.5f,  0.5f, 0.0f,  // Top Right
//        0.5f, -0.5f, 0.0f,  // Bottom Right
//        -0.5f, -0.5f, 0.0f,  // Bottom Left
//        -0.5f,  0.5f, 0.0f   // Top Left
//    };
//    unsigned int indices[] = {  // Note that we start from 0!
//        0, 1, 3,   // First Triangle
//        1, 2, 3    // Second Triangle
//    };
    
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
//    GLuint ebo = 0;
//    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    
    while (!glfwWindowShouldClose(window)) {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3*12);
        glBindVertexArray(0);
        
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
