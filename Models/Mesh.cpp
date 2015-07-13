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

Texture::Texture(GLuint id, const aiString& path) {
    this->id = id;
    this->path = path;
}

Texture::Texture(GLuint id) {
    this->id = id;
}
void Mesh::render(GLuint shader) {
    // Draw mesh
    glBindVertexArray(vao);
    if (hasEbo) {
        glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertices->size());
    }
    glBindVertexArray(0);
}

vector<Vertex> * Vertex::toVertices(float * positions, unsigned int numVertices) {
    vector<Vertex> * vertices = new vector<Vertex>();
    for (int i = 0; i < numVertices; i++) {
        vertices->push_back(Vertex(positions[i * 3],
                                   positions[i * 3 + 1],
                                   positions[i * 3 + 2]));
    }
    return vertices;
}

Mesh::Mesh(vector<Vertex> * v) : Mesh::Mesh(v, NULL) {

}

Mesh::Mesh(vector<Vertex> * v, vector<unsigned int> * i) {
    this->vertices = v;
    this->indices = i;
    hasEbo = indices != NULL && !indices->empty();
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(vertices->at(0)), GL_STATIC_DRAW);

    if (hasEbo) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &(indices->at(0)), GL_STATIC_DRAW);
    }
    
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    
    glBindVertexArray(0);
}