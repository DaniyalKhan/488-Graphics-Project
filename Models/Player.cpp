//
//  Player.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Player.h"



Player::Player(const string& path, GLuint shader, Animation * a) : Character(path, shader) {
    
    
    this->anim = a;
    
    vector<Vertex> * vertices = new vector<Vertex>();
    
    glm::vec3 a1 = getMax();
    glm::vec3 a2 = getMin();
        glm::vec3 tb1 = a1;
        glm::vec3 tb2 = a2;
        vertices->push_back(Vertex(tb1.x, tb1.y, tb1.z));
        vertices->push_back(Vertex(tb1.x, tb1.y, tb2.z));
        vertices->push_back(Vertex(tb1.x, tb2.y, tb1.z));
        vertices->push_back(Vertex(tb1.x, tb2.y, tb2.z));
        
        vertices->push_back(Vertex(tb2.x, tb1.y, tb1.z));
        vertices->push_back(Vertex(tb2.x, tb1.y, tb2.z));
        vertices->push_back(Vertex(tb2.x, tb2.y, tb1.z));
        vertices->push_back(Vertex(tb2.x, tb2.y, tb2.z));
    vector<unsigned int> * indices = new vector<unsigned int>();

    //z = -1
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(3);
    
    //z = 1
    indices->push_back(4);
    indices->push_back(5);
    indices->push_back(6);
    indices->push_back(5);
    indices->push_back(6);
    indices->push_back(7);
    
    //x = 1
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(4);
    indices->push_back(1);
    indices->push_back(4);
    indices->push_back(5);

    //x = -1
    indices->push_back(2);
    indices->push_back(3);
    indices->push_back(6);
    indices->push_back(3);
    indices->push_back(6);
    indices->push_back(7);
    
    //y = 1
    indices->push_back(0);
    indices->push_back(2);
    indices->push_back(4);
    indices->push_back(2);
    indices->push_back(4);
    indices->push_back(6);

    //y = -1
    indices->push_back(3);
    indices->push_back(5);
    indices->push_back(7);
    indices->push_back(3);
    indices->push_back(5);
    indices->push_back(1);
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            glm::vec3 norm;
            if (i == 0) norm = glm::vec3(0, 0, -1);
            else if (i == 1) norm = glm::vec3(0, 0, 1);
            else if (i == 2) norm = glm::vec3(1, 0, 0);
            else if (i == 3) norm = glm::vec3(-1, 0, 0);
            else if (i == 4) norm = glm::vec3(1, 0, 0);
            else if (i == 5) norm = glm::vec3(-1, 0, 0);
            
            normals[i * 18 + j * 3] = norm.x;
            normals[i * 18 + j * 3 + 1] = norm.y;
            normals[i * 18 + j * 3 + 2] = norm.z;
            
        }
    }
    
    vector<Vertex> * finalVertices = new vector<Vertex>();
    for (int i = 0; i < indices->size(); i ++) {
        Vertex v = vertices->at(indices->at(i));
        v.normal.x = normals[indices->at(i) * 3];
        v.normal.y = normals[indices->at(i) * 3 + 1];
        v.normal.z = normals[indices->at(i) * 3 + 2];
        finalVertices->push_back(v);
    }
    
    
    m = new Mesh(finalVertices);
    
}

void Player::render() {
    Model::render();
}


void Player::renderReflection() {
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(glm::scale(modelMatrix(), glm::vec3(2,2,2))));
    m->render(shader);
}