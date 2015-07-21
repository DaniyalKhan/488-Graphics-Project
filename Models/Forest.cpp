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
    
    shears = new vector<glm::mat4>();
    for (int i = 0; i < positions->size(); i++) {
        shears->push_back(glm::mat4());
    }
    
    cuts = new vector<bool>();
    for (int i = 0; i < positions->size(); i++) {
        cuts->push_back(false);
    }
    
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
    for (int i = 0; i < positions->size(); i++) {
        glUniformMatrix4fv(glGetUniformLocation(shader, "translation"), 1, GL_FALSE, glm::value_ptr(positions->at(i)));
//        tree->render();
        vector<Mesh * > * meshes = tree->meshes;
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(tree->modelMatrix()));
        for(int j = 0; j < meshes->size(); j++) {
            if (j == 1 || (j == 0 && !cuts->at(i))) {
                meshes->at(j)->render(shader);
            }
        }
//            m->render(shader);
    }
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

bool Forest::shake(glm::vec3 view, glm::vec3 pos, glm::vec3 &out, int &idx) {
//    glm::mat4 trans = glm::scale(tree->modelMatrix(), glm::vec3(0.5));
    Mesh * trunk = tree->meshes->at(0);
    if (view.y == 0) view.y = 1;
    glm::mat4 trans = glm::scale(tree->modelMatrix(), glm::vec3(1, 2, 1));
    int i = 0;
    float minIdx = -1;
    float min = FLT_MAX;
    for(vector<glm::mat4>::iterator it = positions->begin(); it != positions->end(); ++it) {
        float t;
        glm::vec3 tb2 = ((*it) * trans * glm::vec4(trunk->getMin(), 1)).xyz();
        glm::vec3 tb1 = ((*it) * trans * glm::vec4(trunk->getMax(), 1)).xyz();
        bool hit = rayBox(tb2, tb1, pos, view, t);
        if (hit) {
            if (t < min) {
                min = t;
                minIdx = i;
            }
        }
        i++;
    }
    if (minIdx != -1) {
        if (cuts->at(minIdx)) return false;
        out = (positions->at(minIdx) * glm::vec4(0, 0, 0, 1)).xyz();
//        shears->at(minIdx) = glm::translate(glm::mat4(), glm::vec3(0, 30, 0));
        glm::vec3 axis = glm::cross(normalize(view), glm::vec3(0, 1, 0));
        shears->at(minIdx)[0] = glm::vec4(1, axis.x, axis.x, 0);
        shears->at(minIdx)[1] = glm::vec4(axis.y, 1, axis.y, 0);
        shears->at(minIdx)[2] = glm::vec4(axis.z, axis.z, 1, 0);
        idx = minIdx;
        return true;
    }
    return false;
}

void Forest::cut(int idx) {
    cuts->at(idx) = true;
}

bool Forest::intersect(glm::vec3 pos, glm::vec3 dir, float &t) {
    Mesh * trunk = tree->meshes->at(0);
    glm::mat4 trans = glm::scale(tree->modelMatrix(), glm::vec3(0.8));
    for(vector<glm::mat4>::iterator it = positions->begin(); it != positions->end(); ++it) {
        glm::vec3 tb2 = ((*it) * trans * glm::vec4(trunk->getMin(), 1)).xyz();
        glm::vec3 tb1 = ((*it) * trans * glm::vec4(trunk->getMax(), 1)).xyz();
        if (rayBox(tb2, tb1, pos, dir, t)) {
            return true;
        }
    }
    return false;
}
