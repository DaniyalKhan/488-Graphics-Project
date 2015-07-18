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
    for (int i = 0; i < 28; i++) {
        indices->push_back(i % 24);
        indices->push_back((i+1) % 8);
        indices->push_back((i + 2) % 8);
    }
    
    m = new Mesh(vertices, indices);
    for (int i = 0; i < m->vertices->size(); i++) {
        m->vertices->at(i).color = glm::vec3(0.5,0.5,0.5);
    }
    glBindVertexArray(m->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(GL_ARRAY_BUFFER, m->vertices->size() * sizeof(Vertex), &(m->vertices->at(0)), GL_STATIC_DRAW);
    glBindVertexArray(m->vao);
    
}

void Player::render() {
    Model::render();
//    m->render(shader);
}