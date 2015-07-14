//
//  Landscape.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-11.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Landscape.h"
#include <iostream>
using namespace std;

Landscape::Landscape(int w, int h, float sc, GLuint s)
: width(w), height(h), shader(s), scale(sc) {
    vector<Vertex> * vertices = new vector<Vertex>();
    PerlinNoise noise(width + 1, height + 1);
    noise.setProperties(0.45f, 20.0f);
    float ** pn = noise.perlinNoise(6);
    for (int i = 0; i < width + 1; i++) {
        for (int j = 0; j < height + 1; j++) {
//                        Vertex v((i - width/2) * scale, 1, (j - height/2) * scale);

            Vertex v((i - width/2) * scale, pn[i][j] * scale, (j - height/2) * scale);
            v.texCoords.x = (float)i/(width/10);
            v.texCoords.y = (float)j/(height/10);
            v.normal = glm::vec3(i, pn[i][j], j);
            vertices->push_back(v);
        }
    }
    vector<unsigned int> * indices = new vector<unsigned int>();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int index = i * (width + 1) + j;
            indices->push_back(index);
            indices->push_back(index + width + 1);
            indices->push_back(index + width + 2);
            indices->push_back(index);
            indices->push_back(index + 1);
            indices->push_back(index + width + 2);
        }
    }
    vector<Texture> * textures = new vector<Texture>();
    textures->push_back(Texture(TextureFromFile("grass_green_d.jpg", "Resources/Textures")));
    textures->push_back(Texture(TextureFromFile("mntn_x1_s.jpg", "Resources/Textures/")));
    mesh = new TexturedMesh(vertices, indices, textures);
    
}

glm::vec3 Landscape::positionAt(float x, float z) {
//    return mesh->vertices->at(x * (width + 1) + z).position;
    int i = round(x/scale + width/2.0);
    int j = round(z/scale + height/2.0);
//    if (abs(i - (x/scale + width/2.0)) <= epsilon || abs(j - (z/scale + height/2.0)) <= epsilon) {
//        return mesh->vertices->at(i * (width + 1) + j).position;
//    }
    int i2, j2, i3, j3;
    if (i < x/scale + width/2.0) {
        i2 = i + 1;
    } else {
        i2 = i - 1;
    }
    if (j < z/scale + height/2.0) {
        j3 = j + 1;
    } else {
        j3 = j - 1;
    }
    j2 = j;
    i3 = i;
    glm::vec3 v1 = mesh->vertices->at(i * (width + 1) + j).position;
    glm::vec3 v2 = mesh->vertices->at(i2 * (width + 1) + j2).position;
    glm::vec3 v3 = mesh->vertices->at(i3 * (width + 1) + j3).position;
    
    glm::vec3 n = glm::cross(v1 - v2, v1 - v3);
    
    float d = -glm::dot(n, v1);
    
    glm::vec3 p = glm::vec3(x, -(d + n.x*x + n.z*z)/n.y, z);
    
//    float A = 0.5 * glm::length(glm::cross(v1 - v2, v1 - v3));
//    float A1 = 0.5 * glm::length(glm::cross(p - v2, p - v3));
//    float A2 = 0.5 * glm::length(glm::cross(p - v1, p - v3));
//    float A3 = 0.5 * glm::length(glm::cross(p - v1, p - v2));
//    
//    p.y = A1/A * v1y + A2/A * v2y + A3/A * v3y;
    return p;
    
//    return mesh->vertices->at(i * (width + 1) + j).position;
}

void Landscape::render() {
    mesh->render(shader);
}
