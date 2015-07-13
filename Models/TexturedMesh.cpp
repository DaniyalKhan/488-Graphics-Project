//
//  TexturedMesh.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-01.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "TexturedMesh.h"

TexturedMesh::TexturedMesh(vector<Vertex> * v, vector<unsigned int> * i, vector<Texture> * t) : Mesh(v, i) {
    this->textures = t;
    glBindVertexArray(vao);
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
    glBindVertexArray(0);
}

void TexturedMesh::render(GLuint shader) {
    for(GLuint i = 0; i < textures->size(); i++)
    {
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        ss << "texture";
        ss << (i + 1);
        // Now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader, ss.str().c_str()), i);
        // Active proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures->at(i).id);
    }
    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    //    glUniform1f(glGetUniformLocation(shader, "material.shininess"), 16.0f);
    Mesh::render(shader);
    // Always good practice to set everything back to defaults once configured.
    for (GLuint i = 0; i < textures->size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}