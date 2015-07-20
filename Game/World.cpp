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
    grassA = new Model("Resources/Models/Flower/Cloud Flower.obj", false);
    grassA->scale(glm::vec3(0.05, 0.05, 0.05));
    grassB = new Model("grass/6.3ds");
    grassA->setShader(textureModelShader);
    grassB->setShader(textureModelShader);
    
    vector<glm::mat4> * trees = new vector<glm::mat4>();
    srand (time(NULL));
    for (int i = 0; i < 50; i++) {
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
    
    characters = new vector<Character * >();
    
    fallers = new vector<Character * >();
    
    jumpers = new vector<Character * >();
    int numJumpers = 20;
    float jscale = scale * 0.75;
    for (int i = 0; i < numJumpers; i++) {
        double r1 = (double)rand()/RAND_MAX * 2 - 1;
        double r2 = (double)rand()/RAND_MAX * 2 - 1;
        glm::vec3 pos = glm::vec3(r1 * terrainWidth/2 * jscale, 0, r2 * terrainHeight/2 * jscale);
        Character * c = new Character("Resources/Models/Pikachu/Pikachu.dae", textureModelShader, new JumpAnimation());
        c->translate(landscape->positionAt(pos.x, pos.z));
        c->strafe((double)rand()/RAND_MAX * 360);
        jumpers->push_back(c);
        characters->push_back(c);
    }
    
    flyers = new vector<Character * >();
    rotations = new vector<glm::vec3>();
    int numFlyers = 10;
    float fscale = scale * 0.75;
    for (int i = 0; i < numFlyers; i++) {
        double r1 = (double)rand()/RAND_MAX * 2 - 1;
        double r2 = (double)rand()/RAND_MAX * 2 - 1;
        glm::vec3 rPos = glm::vec3(r1 * terrainWidth/2 * fscale, 0, r2 * terrainHeight/2 * fscale);
        rotations->push_back(rPos);
        Character * f = new Character("Resources/Models/Pidgeotto/Pidgeotto.dae",
        textureModelShader, new FlyAnimation(90 * (double)rand()/RAND_MAX, rPos));
        double c = 20 * (double)rand()/RAND_MAX;
        f->translate(landscape->positionAt(rPos.x, rPos.z) + glm::vec3(c, 50, c));
        if (rPos.x > 0)  {
            if (rPos.z < 0) {
                f->strafe(270.0f);
            } else  {
                f->strafe(180.0f);
            }
        }
        if (rPos.x < 0)  {
            if (rPos.z > 0) {
                 f->strafe(90.0f);
            }
        }
        f->rot(30.0f, glm::vec3(0,1, 0));
        flyers->push_back(f);
        characters->push_back(f);
    }
    
    ui = new UI(manager.manageShader(SHADER_UI, "UI"), width, height);
    ui->setText("TESTING", 0, 0.8, false);
    
    lastTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    
//    play();
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
    
    for (int i = 0; i < jumpers->size(); i++) {
        float distance = 1.0/4.0;
        Character * jumper = jumpers->at(i);
        
        glm::vec3 oldPos = jumper->position();
        
        float y = landscape->positionAt(jumper->position().x, jumper->position().z).y;
        jumper->forward(distance, deltaTime/200.0);
        
        if (landscape->outofBounds(jumper->position().x, jumper->position().z)) {
            float r = ((double)rand()/RAND_MAX * 2 - 1) * 45;
            jumper->strafe(180 + r);
            jumper->translate(oldPos - jumper->position());
        } else {
            float delta = landscape->positionAt(jumper->position().x, jumper->position().z).y - y;
            jumper->addHeight(delta);
        }
        
    }

    for (int i = 0; i < fallers->size(); i++) {
        Character * faller = fallers->at(i);
        faller->forward(0, deltaTime);
    }
    
    for (int i = 0; i < flyers->size(); i++) {
        Character * flyer = flyers->at(i);
        flyer->forward(0, deltaTime);
//        glm::vec2 v = normalize(glm::vec2(flyer->viewDirection().x, flyer->viewDirection().z));
//        glm::vec2 p = normalize(glm::vec2(flyer->position().x, flyer->position().z));
//        float angle = glm::angle(v, p);
//        flyer->rot(angle, glm::vec3(0, 0, 1));
    }
    
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
        
        if (*lastKey == GLFW_KEY_SPACE) {
            glm::vec3 view;
            if (firstPerson) view = camera->getView();
            else view = player->viewDirection();
            glm::vec3 treePos;
            if(forest->shake(view, player->position(), treePos)) {
                float height = 5;
                glm::vec3 d = normalize(player->position() - treePos);
                d.y = 5;
                treePos.y += height;
                Character * f = new Character("Resources/Models/Kakuna/Kakuna.dae", manager.retrieveShader(SHADER_TEXTURED_MODEL), new FallAnimation(d, 5, height));
                f->translate(treePos);
                fallers->push_back(f);
                characters->push_back(f);
                keyboard->remove(GLFW_KEY_SPACE);
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
                if (dot(camera->getView(), player->viewDirection()) < 0) {
                    camera->spin(-(float)(deltaTime * 90.0f/1000.0f));
                }
            } else if (*lastKey == GLFW_KEY_DOWN) {
                camera->spin(-(float)(deltaTime * 90.0f/1000.0f));
                if (dot(camera->getView(), player->viewDirection()) < 0) {
                    camera->spin((float)(deltaTime * 90.0f/1000.0f));
                }
            } else if (*lastKey == GLFW_KEY_LEFT) {
                camera->rotate(-(float)(deltaTime * 90.0f/1000.0f));
                if (dot(camera->getView(), player->viewDirection()) < 0) {
                    camera->rotate((float)(deltaTime * 90.0f/1000.0f));
                }
            } else if (*lastKey == GLFW_KEY_RIGHT) {
                camera->rotate((float)(deltaTime * 90.0f/1000.0f));
                if (dot(camera->getView(), player->viewDirection()) < 0) {
                    camera->rotate(-(float)(deltaTime * 90.0f/1000.0f));
                }
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
    glViewport(0, 0, width, height);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDepthFunc(GL_LEQUAL);
    bindShader(SHADER_SKY);
    skybox->render();
    glDepthFunc(GL_LESS);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_dLINE);
  
    glm::vec3 viewPos = camera->getPosition();
    glm::vec3 lightPos = glm::vec3(-100, 600, 100);
    
    GLuint modelShader = bindShader(SHADER_TEXTURED_MODEL);
    glUniform3f(glGetUniformLocation(modelShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(modelShader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    player->render();
    for (int i = 0; i < jumpers->size(); i++) {
        jumpers->at(i)->render();
    }
    for (int i = 0; i < flyers->size(); i++) {
        flyers->at(i)->render();
    }
    for (int i = 0; i < fallers->size(); i++) {
        fallers->at(i)->render();
    }
    for (int i = 0; i < grassPositions->size()/3; i++) {
//        if (rand()/RAND_MAX < 0.5f) {
            grassA->resetTranslation();
            grassA->translate(grassPositions->at(i));
            grassA->render();
//        } else {
//            grassB->resetTranslation();
//            grassB->translate(grassPositions->at(i));
//            grassB->render();
//        }
    }
    
    bindShader(SHADER_GROUND);
    landscape->render();

    GLuint treeShader = bindShader(SHADER_TREE);
    glUniform3f(glGetUniformLocation(treeShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(treeShader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    forest->render();
    
    
    if (firstPerson) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_BLEND );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        bindShader(SHADER_UI);
        
        float t = FLT_MAX;
        float min = FLT_MAX;
        bool hit = false;
        float idx = -1;
        string path = "";
        GLuint shader;
        for (int i = 0; i < characters->size(); i++) {
            Character * c = characters->at(i);
            glm::vec3 b1 = (c->modelMatrix() * glm::vec4(c->getMin(), 1)).xyz();
            glm::vec3 b2 = (c->modelMatrix() * glm::vec4(c->getMax(), 1)).xyz();
            
            if (b1.x > b2.x) {
                float temp = b2.x;
                b2.x = b1.x;
                b1.x = temp;
            }
            
            if (b1.y > b2.y) {
                float temp = b2.y;
                b2.y = b1.y;
                b1.y = temp;
            }
            
            if (b1.z > b2.z) {
                float temp = b2.z;
                b2.z = b1.z;
                b1.z = temp;
            }
            
            if (rayBox(b1, b2, camera->getPosition(), camera->getView(), t)) {
                if (t < min && t > 0) {
                    min = t;
                    idx = i;
                    hit = true;
                    path = characters->at(idx)->path;
                    shader = modelShader;
                }
            }
        }
        
        if (forest->intersect(camera->getPosition(), camera->getView(), t)) {
            if (t < min && t > 0) {
                min = t;
                hit = true;
                path = forest->tree->path;
                shader = treeShader;
            }
        }
        
        if (hit) {
            ui->render(true, path, shader);
        } else {
            ui->render(false, "", shader);
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