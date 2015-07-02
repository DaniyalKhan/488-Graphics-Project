//
//  Mesh.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-24.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Mesh__
#define __Project__Mesh__

#include <glew.h>
#include <OpenGL/gl3.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/scene.h>

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v);
    Vertex(float x, float y, float z);
    static vector<Vertex> * toVertices(float * positions, unsigned int numVertices);
};

struct Texture {
    GLuint id;
    string type;
    aiString path;
    Texture(GLuint id, const string& type, const aiString& path);
};

class Mesh {
protected:
    vector<Vertex> * vertices;
    vector<unsigned int> * indices;
    GLuint vao, vbo, ebo;
    bool hasEbo;
public:
    Mesh(vector<Vertex> * v);
    Mesh(vector<Vertex> * v, vector<unsigned int> * i);
    virtual void render(GLuint shader);
};

#endif /* defined(__Project__Mesh__) */
