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
    // Bind appropriate textures
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < textures->size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = textures->at(i).type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();
        // Now set the sampler to the correct texture unit
        glUniform1f(glGetUniformLocation(shader, (name + number).c_str()), i);
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