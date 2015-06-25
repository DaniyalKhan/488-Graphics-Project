//
//  Mesh.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-24.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Mesh.h"
#include <sstream>

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

Texture::Texture(GLuint id, const string& type, const aiString& path) {
    this->id = id;
    this->type = type;
    this->path = path;
}

void Mesh::render(GLuint shader) {

    // Bind appropriate textures
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
//    for(GLuint i = 0; i < textures->size(); i++)
//    {
//        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
//        // Retrieve texture number (the N in diffuse_textureN)
//        stringstream ss;
//        string number;
//        string name = textures->at(i).type;
//        if(name == "texture_diffuse")
//            ss << diffuseNr++; // Transfer GLuint to stream
//        else if(name == "texture_specular")
//            ss << specularNr++; // Transfer GLuint to stream
//        number = ss.str();
//        // Now set the sampler to the correct texture unit
//        glUniform1f(glGetUniformLocation(shader, (name + number).c_str()), i);
//        // And finally bind the texture
//        glBindTexture(GL_TEXTURE_2D, textures->at(i).id);
//    }
    
    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
//    glUniform1f(glGetUniformLocation(shader, "material.shininess"), 16.0f);
    
    // Draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    // Always good practice to set everything back to defaults once configured.
//    for (GLuint i = 0; i < textures->size(); i++)
//    {
//        glActiveTexture(GL_TEXTURE0 + i);
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }

}

Mesh::Mesh(vector<Vertex> * v, vector<unsigned int> * i, vector<Texture> * t) {
    this->vertices = v;
    this->indices = i;
    this->textures = t;
    
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
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(unsigned int), (GLvoid*)0);
    // Vertex Texture Coords
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
    
    glBindVertexArray(0);
}