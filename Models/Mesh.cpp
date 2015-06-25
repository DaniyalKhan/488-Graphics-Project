//
//  Mesh.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-24.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Mesh.h"

void Mesh::render(GLuint shader) {
    // Bind appropriate textures
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    //    for(GLuint i = 0; i < this->textures.size(); i++)
    //    {
    //        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
    //        // Retrieve texture number (the N in diffuse_textureN)
    //        stringstream ss;
    //        string number;
    //        string name = this->textures[i].type;
    //        if(name == "texture_diffuse")
    //            ss << diffuseNr++; // Transfer GLuint to stream
    //        else if(name == "texture_specular")
    //            ss << specularNr++; // Transfer GLuint to stream
    //        number = ss.str();
    //        // Now set the sampler to the correct texture unit
    //        glUniform1f(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
    //        // And finally bind the texture
    //        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    //    }
    
    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    //    glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);
    
    // Draw mesh
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, nVertices/3);
//    glDrawElements(GL_TRIANGLES, this->nIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    // Always good practice to set everything back to defaults once configured.
//    for (GLuint i = 0; i < this->textures.size(); i++)
//    {
//        glActiveTexture(GL_TEXTURE0 + i);
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }
}

Mesh::Mesh(float * v, unsigned int nv, unsigned int * i, unsigned int ni, Texture * t, unsigned int nt)
{
    this->vertices = v;
    this->indices = i;
    this->textures = t;
    this->nVertices = nv;
    this->nIndices = ni;
    this->nTextures = nt;
    
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);
    
    glBindVertexArray(this->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(float), vertices, GL_STATIC_DRAW);
    
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
    // Vertex Normals
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
    
    glBindVertexArray(0);
}