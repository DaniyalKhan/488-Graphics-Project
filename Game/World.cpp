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
    
    Vertex v[4] = {
        Vertex(100, 0, 100),
        Vertex(100, 0, -100),
        Vertex(-100, 0, 100),
        Vertex(-100, 0, -100),
    };
    
    vector<unsigned int> * indices = new vector<unsigned int>({0,1,2,1,2,3});
    vector<Vertex> * groundVertices = new vector<Vertex>(std::begin(v), std::end(v));

    mesh = new Mesh(groundVertices, indices);
    
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
    
//        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    GLuint modelShader = bindShader(SHADER_TEXTURED_MODEL);
    glUniformMatrix4fv(glGetUniformLocation(modelShader, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix * player->modelMatrix()));
    player->render();

//    GLuint groundShader = bindShader(SHADER_GROUND);
//    glUniformMatrix4fv(glGetUniformLocation(groundShader, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
//    mesh->render(groundShader);
    
    glDepthFunc(GL_LEQUAL);
    GLuint skyShader = bindShader(SHADER_SKY);
    glUniformMatrix4fv(glGetUniformLocation(skyShader, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
        skybox->render();
    glDepthFunc(GL_LESS);

//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    GLuint groundShader = bindShader(SHADER_GROUND);
    glUniformMatrix4fv(glGetUniformLocation(groundShader, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
    landscape->render();
    

}

GLuint World::bindShader(int shaderKey) {
    GLuint modelShader = manager.retrieveShader(shaderKey);
    glUseProgram(modelShader);
    return modelShader;
}