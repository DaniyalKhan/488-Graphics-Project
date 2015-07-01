//
//  World.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include <glew.h>
#include "World.h"

World::World(GLFWwindow * window) {
    glm::vec3 startingPosition = glm::vec3(0, 0, 0);
    keyboard = new Keyboard(window);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    projectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width)/height, 0.01f, 100.0f);
    manager.manageShader(SHADER_TEXTURED_MODEL, "ModelShaders/shader");
    
    GLuint textureModelShader = manager.retrieveShader(SHADER_TEXTURED_MODEL);
    
    player = new Player("bulbasaur.dae", textureModelShader);
    player->translate(startingPosition);
    camera = new Camera(startingPosition);
    
    lastTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

void World::update() {
    unsigned long deltaTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1) - lastTime;
    lastTime = lastTime + deltaTime;
    int * lastKey = keyboard->queryInput();
    if (lastKey != NULL) {
        
        if (*lastKey == GLFW_KEY_UP) {
            camera->roll((float)(deltaTime * 90.0f/1000.0f));
        } else if (*lastKey == GLFW_KEY_DOWN) {
            camera->roll(-(float)(deltaTime * 90.0f/1000.0f));
        } else if (*lastKey == GLFW_KEY_LEFT) {
            camera->pan(-(float)(deltaTime * 90.0f/1000.0f));
        } else if (*lastKey == GLFW_KEY_RIGHT) {
            camera->pan((float)(deltaTime * 90.0f/1000.0f));
        }
        
        if (*lastKey == GLFW_KEY_V) {
            camera->look(player->viewDirection(), player->position());
        }
        
    }
}

void World::render() {
    glm::mat4 viewProjectionMatrix = projectionMatrix * camera->getViewMatrix();
    GLuint modelShader = bindShader(SHADER_TEXTURED_MODEL);
    glUniformMatrix4fv(glGetUniformLocation(modelShader, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix * player->modelMatrix()));
    player->render();
}

GLuint World::bindShader(int shaderKey) {
    GLuint modelShader = manager.retrieveShader(SHADER_TEXTURED_MODEL);
    glUseProgram(modelShader);
    return modelShader;
}