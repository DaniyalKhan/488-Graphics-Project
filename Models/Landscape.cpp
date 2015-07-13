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
    noise.setProperties(0.5f, 15.0f);
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

glm::vec3 Landscape::positionAt(int x, int z) {
    return mesh->vertices->at(x * (width + 1) + z).position;
}

void Landscape::render() {
    mesh->render(shader);
}
