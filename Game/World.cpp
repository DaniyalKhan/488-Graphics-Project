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
    keyboard = new Keyboard(window);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    projectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width)/height, 0.01f, 1000.0f);
    
    landscape = new Landscape(100, 100, 20, manager.manageShader(SHADER_GROUND, "ModelShaders/Ground"));
    
    GLuint textureModelShader = manager.manageShader(SHADER_TEXTURED_MODEL, "ModelShaders/SimpleModel");
    
    glm::vec3 startingPosition = landscape->positionAt(50, 50);
    player = new Player("bulbasaur.dae", textureModelShader);
    player->translate(startingPosition);
    camera = new Camera(startingPosition);
    
    vector<const GLchar*> faces;
    faces.push_back("Resources/Skyboxes/Sky/right.jpg");
    faces.push_back("Resources/Skyboxes/Sky/left.jpg");
    faces.push_back("Resources/Skyboxes/Sky/top.jpg");
    faces.push_back("Resources/Skyboxes/Sky/bottom.jpg");
    faces.push_back("Resources/Skyboxes/Sky/back.jpg");
    faces.push_back("Resources/Skyboxes/Sky/front.jpg");
    
    skybox = new Skybox(faces, manager.manageShader(SHADER_SKY, "ModelShaders/SimpleSky"));
    
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
    
    bindShader(SHADER_TEXTURED_MODEL);
    player->render();
    
    glDepthFunc(GL_LEQUAL);
    bindShader(SHADER_SKY);
    skybox->render();
    glDepthFunc(GL_LESS);
    
    bindShader(SHADER_GROUND);
    landscape->render();
    
}

GLuint World::bindShader(int shaderKey) {
    GLuint shader = manager.retrieveShader(shaderKey);
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, value_ptr(camera->getViewMatrix()));
    return shader;
}