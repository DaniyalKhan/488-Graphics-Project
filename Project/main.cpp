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
#include <glm/gtx/transform.hpp>
#include "glm/ext.hpp"

#include "Loader.h"
#include "World.h"

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
    
    float width = 1280.0f;
    float height = 720.0f;
    
    GLFWwindow* window = glfwCreateWindow(width, height, "CS488 Project", NULL, NULL);
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
    
    
    glViewport(0, 0, width, height);

    GLuint simpleShader = LoadShader("Shaders/shader.vert", "Shaders/shader.frag");
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
    glm::vec3 lookAt = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1.0f, 0);
    
    glm::mat4 view = glm::lookAt(cameraPos, lookAt, up);
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width/height, 0.01f, 100.0f);
        glm::mat4 Scale = glm::scale(glm::mat4(), glm::vec3(10.0f, 10.0f, 10.0f));
    glm::mat4 Translate = glm::translate(glm::mat4(), glm::vec3(0.0f, -5.0f, 0.0f));
    glm::mat4 viewProjection = projection * view;
    
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    
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
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glBindVertexArray(0);

    World w(window);
    
    while (!glfwWindowShouldClose(window)) {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        w.update();
        w.render();
        
//        glUseProgram(simpleShader);
//        glUniformMatrix4fv(glGetUniformLocation(simpleShader, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(projection * w.camera->getViewMatrix() * Translate * Scale));
        
//        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLES, 0, 12*3);
//        glBindVertexArray(0);
        
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
