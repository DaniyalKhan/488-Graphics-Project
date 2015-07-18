//
//  Forest.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-13.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#define GLM_SWIZZLE
#include "Forest.h"

Forest::Forest(GLuint s, vector<glm::mat4> * translations) : shader(s) {
//    tree = new Model("ModelFiles/tree/t2.obj");
    tree = new Model("ModelFiles/LowPolyTree/tree.obj", false);
    tree->setShader(shader);
    positions = translations;
    tree->scale(glm::vec3(4, 4, 4));
    tree->applyColor(glm::vec3(0.0, 0.6f, 0.3f), 0);
//    tree->applyColor(glm::vec3(0.22, 0.68f, 0.28f), 2);
//    tree->applyColor(glm::vec3(0.22, 0.68f, 0.28f), 3);
//    tree->applyColor(glm::vec3(0.22, 0.68f, 0.28f), 4);
//    tree->applyColor(glm::vec3(0.22, 0.68f, 0.28f), 5);
    tree->applyColor(glm::vec3(0.68f, 0.51f, 0.22f), 1);
    
    vector<Vertex> * vertices = new vector<Vertex>();
    
    Mesh * trunk = tree->meshes->at(1);
    glm::vec3 a1 = ( glm::vec4(trunk->getMin(), 1)).xyz();
    glm::vec3 a2 = ( glm::vec4(trunk->getMax(), 1)).xyz();
    for(vector<glm::mat4>::iterator it = positions->begin(); it != positions->end(); ++it) {
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
    }
    vector<unsigned int> * indices = new vector<unsigned int>();
    for (int i = 0; i < 32; i++) {
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

void Forest::render() {
    for(vector<glm::mat4>::iterator it = positions->begin(); it != positions->end(); ++it) {
        glUniformMatrix4fv(glGetUniformLocation(shader, "translation"), 1, GL_FALSE, glm::value_ptr(*it));
        tree->render();
//            m->render(shader);
    }
}

bool pointInBox(glm::vec3 b1, glm::vec3 b2, float x, float y, float z, glm::mat4 transform) {
    
    glm::vec3 v = (transform * glm::vec4(x, y, z, 1)).xyz();
    x = v.x; y = v.y; z = v.z;
    
    float xmin = fmin(b1.x, b2.x);
    float xmax = fmax(b1.x, b2.x);
    
    float ymin = fmin(b1.y, b2.y);
    float ymax = fmax(b1.y, b2.y);
    
    float zmin = fmin(b1.z, b2.z);
    float zmax = fmax(b1.z, b2.z);
    
    if (x < xmin || x > xmax) return false;
    if (y < ymin || y > ymax) return false;
    if (z < zmin || z > zmax) return false;
    
    return true;
}

bool Forest::intersect(glm::vec3 b1, glm::vec3 b2, glm::mat4 t) {
    Mesh * trunk = tree->meshes->at(0);
    glm::mat4 trans = glm::scale(tree->modelMatrix(), glm::vec3(0.5));
    for(vector<glm::mat4>::iterator it = positions->begin(); it != positions->end(); ++it) {
        glm::vec3 tb2 = ((*it) * trans * glm::vec4(trunk->getMin(), 1)).xyz();
        glm::vec3 tb1 = ((*it) * trans * glm::vec4(trunk->getMax(), 1)).xyz();
        if (pointInBox(tb1, tb2, b1.x, b1.y, b1.z, t)) return true;
        if (pointInBox(tb1, tb2, b1.x, b1.y, b2.z, t)) return true;
        if (pointInBox(tb1, tb2, b1.x, b2.y, b1.z, t)) return true;
        if (pointInBox(tb1, tb2, b1.x, b2.y, b2.z, t)) return true;
        
        if (pointInBox(tb1, tb2, b2.x, b1.y, b1.z, t)) return true;
        if (pointInBox(tb1, tb2, b2.x, b1.y, b2.z, t)) return true;
        if (pointInBox(tb1, tb2, b2.x, b2.y, b1.z, t)) return true;
        if (pointInBox(tb1, tb2, b2.x, b2.y, b2.z, t)) return true;
    }
    return false;
}