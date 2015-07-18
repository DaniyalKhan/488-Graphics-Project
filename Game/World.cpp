//
//  World.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#define GLM_SWIZZLE
#include <glew.h>
#include <iostream>
#include "World.h"

World::World(GLFWwindow * window) {
    keyboard = new Keyboard(window);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    projectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width)/height, 0.01f, 400.0f);
    
    int terrainWidth = 100;
    int terrainHeight = 100;
    int scale = 5;
    landscape = new Landscape(terrainWidth, terrainHeight, scale, manager.manageShader(SHADER_GROUND, "ModelShaders/Ground"));
    
    GLuint textureModelShader = manager.manageShader(SHADER_TEXTURED_MODEL, "ModelShaders/SimpleModel");
    GLuint treeShader = manager.manageShader(SHADER_TREE, "ModelShaders/Tree");

    glm::vec3 startingPosition = landscape->positionAt(0, 0);
    player = new Player("Resources/Models/Bulbasaur/Bulbasaur.dae", textureModelShader, new RotationAnimation(500.0f));

    player->translate(startingPosition);
    camera = new Camera(startingPosition);
    
    grassPositions = new vector<glm::vec3>();
    
    //grass 4, 6
    grassA = new Model("grass/4.3ds");
    grassB = new Model("grass/6.3ds");
    grassA->setShader(textureModelShader);
    grassB->setShader(textureModelShader);
    
    vector<glm::mat4> * trees = new vector<glm::mat4>();
    srand (time(NULL));
    for (int i = 0; i < 25; i++) {
        float x = ((rand() % terrainWidth) - terrainWidth/2) * scale * 0.7;
        float z = ((rand() % terrainHeight) - terrainHeight/2) * scale * 0.7;
        for (int j = 0; j < 4; j++) {
            glm::vec3 localTrans = glm::vec3(15 + rand() % 5, 0, 15 + rand() % 5);
            localTrans = glm::vec3(0, 0, -15);
            float angle = glm::radians((float)(rand() % 360 + rand()));
            angle = 0;
            glm::vec3 rotatedLocalTrans = glm::rotate(localTrans, angle, glm::vec3(0, 1, 0));
            
            glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(x, landscape->positionAt(rotatedLocalTrans.x + x, rotatedLocalTrans.z + z).y, z));
            trans = glm::rotate(trans, angle, glm::vec3(0, 1, 0));
            trans = glm::translate(trans, localTrans);

            trees->push_back(trans);
        }
        grassPositions->push_back(glm::vec3(landscape->positionAt(x, z) + glm::vec3(0,1,0)));
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
    
    jumpers = new vector<Character>();
    
    Character c("Resources/Models/Pikachu/Pikachu.dae", textureModelShader, new JumpAnimation());
    c.translate(landscape->positionAt(20, 20));
    
    jumpers->push_back(c);
    
    ui = new UI(manager.manageShader(SHADER_UI, "UI"), width, height);
    
}

void World::moveCharacter(float direction, float deltaTime) {
    float height = landscape->positionAt(player->position().x, player->position().z).y;
    glm::vec3 trans = player->forward(direction, deltaTime);
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
}

void World::update() {
    unsigned long deltaTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1) - lastTime;
    lastTime = lastTime + deltaTime;
    
//    for (int i = 0; i < jumpers->size(); i++) {
//        float y = jumpers->at(i).position().y;
//        glm::vec3 trans = jumpers->at(i).forward(1.0/10, deltaTime/200.0);
//        float delta = landscape->positionAt(trans.x, trans.z).y - y;
//        jumpers->at(i).addHeight(delta);
//    }
    
    int * lastKey = keyboard->queryInput();
    if (lastKey != NULL) {
        
        glm::vec3 b2 = player->getMin();
        glm::vec3 b1 = player->getMax();
        
        if (*lastKey == GLFW_KEY_D) {
            player->strafe(-2);
        } else if (*lastKey == GLFW_KEY_A) {
            player->strafe(2);
        } else if (*lastKey == GLFW_KEY_W) {
            moveCharacter(0.5, deltaTime);
            if (forest->intersect(b1, b2, player->modelMatrix())) {
                moveCharacter(-0.5, deltaTime);
            }
        } else if (*lastKey == GLFW_KEY_S) {
            moveCharacter(-0.5, deltaTime);
            if (forest->intersect(b1, b2, player->modelMatrix())) {
                moveCharacter(0.5, deltaTime);
            }
        }
        
        if (!firstPerson) {
        
            if (*lastKey == GLFW_KEY_UP) {
                camera->roll((float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_DOWN) {
                camera->roll(-(float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_LEFT) {
                camera->pan(-(float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_RIGHT) {
                camera->pan((float)(deltaTime * 90.0f/1000.0f));
            }
        
        } else {
            if (*lastKey == GLFW_KEY_UP) {
                camera->spin((float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_DOWN) {
                camera->spin(-(float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_LEFT) {
                camera->rotate(-(float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_RIGHT) {
                camera->rotate((float)(deltaTime * 90.0f/1000.0f));
            }
        }
    
        if (*lastKey == GLFW_KEY_V) {
            if (!firstPerson) {
                glm::vec3 max = (player->modelMatrix() * glm::vec4(player->getMax(), 1)).xyz();
                glm::vec3 min = (player->modelMatrix() * glm::vec4(player->getMin(), 1)).xyz();
                glm::vec3 pos = glm::vec3((max.x + min.x)/2, max.y, max.z);
                camera->look(player->viewDirection() + pos, pos);
            } else {
                camera->look(player->position());
            }
            firstPerson = !firstPerson;
            keyboard->remove(GLFW_KEY_V);
        }
        
    }
}

void World::render() {
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDepthFunc(GL_LEQUAL);
    bindShader(SHADER_SKY);
    skybox->render();
    glDepthFunc(GL_LESS);
//
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
    glm::vec3 viewPos = camera->getPosition();
    glm::vec3 lightPos = glm::vec3(-100, 600, 100);
    
    GLuint modelShader = bindShader(SHADER_TEXTURED_MODEL);
    glUniform3f(glGetUniformLocation(modelShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(modelShader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    player->render();
    for (int i = 0; i < jumpers->size(); i++) {
        jumpers->at(i).render();
    }
//    for (int i = 0; i < grassPositions->size()/3; i++) {
//        if (rand()/RAND_MAX < 0.5f) {
//            grassA->resetTranslation();
//            grassA->translate(grassPositions->at(i));
//            grassA->render();
//        } else {
//            grassB->resetTranslation();
//            grassB->translate(grassPositions->at(i));
//            grassB->render();
//        }
//    }
    
    bindShader(SHADER_GROUND);
    landscape->render();

    GLuint treeShader = bindShader(SHADER_TREE);
    glUniform3f(glGetUniformLocation(treeShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(treeShader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    forest->render();
    
    float t;
    
    if (firstPerson) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_BLEND );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        bindShader(SHADER_UI);
        if (forest->intersect(camera->getPosition(), camera->getView(), t))
            ui->render(true);
        else {
            ui->render(false);
        }
        glDisable( GL_BLEND );
    }
    
}

GLuint World::bindShader(int shaderKey) {
    GLuint shader = manager.retrieveShader(shaderKey);
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, value_ptr(camera->getViewMatrix()));
    return shader;
}