//
//  Mesh.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-24.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Mesh.h"

Vertex::Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) {
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    this->normal.x = nx;
    this->normal.y = ny;
    this->normal.z = nz;
    this->texCoords.x = u;
    this->texCoords.y = v;
}

Vertex::Vertex(float x, float y, float z) {
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

Texture::Texture(GLuint id, const string& type, const aiString& path) {
    this->id = id;
    this->type = type;
    this->path = path;
}

void Mesh::render(GLuint shader) {

    // Draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

Mesh::Mesh(vector<Vertex> * v, vector<unsigned int> * i) {
    this->vertices = v;
    this->indices = i;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(vertices->at(0)), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &(indices->at(0)), GL_STATIC_DRAW);
    
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    
    glBindVertexArray(0);
}