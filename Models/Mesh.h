//
//  Mesh.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-24.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Mesh__
#define __Project__Mesh__

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    string type;
};

class Mesh {
private:
    float * vertices;
    unsigned int nVertices;
    
    unsigned int * indices;
    unsigned int nIndices;
    
    Texture * textures;
    unsigned int nTextures;
//    vector<Vertex> vertices;
//    vector<unsigned int> indices;
//    vector<Texture> textures;
    GLuint vao, vbo, ebo;
public:
    Mesh(float * v, unsigned int nv, unsigned int * i, unsigned int ni, Texture * t, unsigned int nt);
    void render(GLuint shader);
};

#endif /* defined(__Project__Mesh__) */
