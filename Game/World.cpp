//
//  World.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include <glew.h>
#include <iostream>
#include "World.h"

World::World(GLFWwindow * window) {
    keyboard = new Keyboard(window);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    projectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width)/height, 0.01f, 1000.0f);
    
    int terrainWidth = 300;
    int terrainHeight = 300;
    int scale = 5;
    landscape = new Landscape(terrainWidth, terrainHeight, scale, manager.manageShader(SHADER_GROUND, "ModelShaders/Ground"));
    
    GLuint textureModelShader = manager.manageShader(SHADER_TEXTURED_MODEL, "ModelShaders/SimpleModel");
    GLuint treeShader = manager.manageShader(SHADER_TREE, "ModelShaders/Tree");

    glm::vec3 startingPosition = landscape->positionAt(0, 0);
    player = new Player("bulbasaur.dae", textureModelShader);

    player->translate(startingPosition);
    camera = new Camera(startingPosition);
    
    vector<glm::mat4> * trees = new vector<glm::mat4>();
    srand (time(NULL));
    for (int i = 0; i < 30; i++) {
        float x = ((rand() % terrainWidth) - terrainWidth/2) * scale * 0.5;
        float z = ((rand() % terrainHeight) - terrainHeight/2) * scale * 0.5;
        for (int j = 0; j < 8; j++) {
            glm::vec3 localTrans = glm::vec3(15 + rand() % 5, 0, 15 + rand() % 5);
            float angle = glm::radians((float)(rand() % 360 + rand()));
            glm::vec3 rotatedLocalTrans = glm::rotate(localTrans, angle, glm::vec3(0, 1, 0));
            
            glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(x, landscape->positionAt(rotatedLocalTrans.x + x, rotatedLocalTrans.z + z).y, z));
            trans = glm::rotate(trans, angle, glm::vec3(0, 1, 0));
            trans = glm::translate(trans, localTrans);

            trees->push_back(trans);
        }
    }
    
    forest = new Forest(treeShader, trees);

    
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

void World::moveCharacter(float direction) {
    float height = landscape->positionAt(player->position().x, player->position().z).y;
    glm::vec3 trans = player->forward(direction);
    float delta = landscape->positionAt(player->position().x, player->position().z).y - height;
    float threshold = 0.5;
    if (delta > threshold) {
        extra += (delta/2);
        delta = delta/2;
    }
    if (extra > 0) {
        delta += fmin(extra, threshold);
        extra -= fmin(extra, threshold);
    }
    player->addHeight(delta);
    trans.y += delta;
    camera->move(trans);
    player->applyColor(glm::vec3(1,1,1), 0);
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
        
        if (*lastKey == GLFW_KEY_D) {
            player->strafe(-2);
        } else if (*lastKey == GLFW_KEY_A) {
            player->strafe(2);
        } else if (*lastKey == GLFW_KEY_W) {
            moveCharacter(0.5);
        } else if (*lastKey == GLFW_KEY_S) {
            moveCharacter(-0.5);
        }
        
//        std::cout << glm::to_string(player->position()) << std::endl;
//        std::cout << glm::to_string(landscape->positionAt(player->position().x, player->position().z)) << std::endl;
        
        
        if (*lastKey == GLFW_KEY_V) {
            camera->look(player->viewDirection(), player->position());
        }
        
    }
}

void World::render() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDepthFunc(GL_LEQUAL);
    bindShader(SHADER_SKY);
    skybox->render();
    glDepthFunc(GL_LESS);
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
    glm::vec3 viewPos = camera->getPosition();
    glm::vec3 lightPos = glm::vec3(100, 600, 100);
    
    GLuint modelShader = bindShader(SHADER_TEXTURED_MODEL);
    glUniform3f(glGetUniformLocation(modelShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(modelShader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    player->render();
    
    bindShader(SHADER_GROUND);
    landscape->render();
    
    GLuint treeShader = bindShader(SHADER_TREE);
    glUniform3f(glGetUniformLocation(treeShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(treeShader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    forest->render();
    
}

GLuint World::bindShader(int shaderKey) {
    GLuint shader = manager.retrieveShader(shaderKey);
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, value_ptr(camera->getViewMatrix()));
    return shader;
}