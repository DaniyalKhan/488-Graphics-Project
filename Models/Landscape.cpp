//
//  Landscape.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-11.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Landscape.h"

Landscape::Landscape(int w, int h, GLuint s)
: width(w), height(h), shader(s) {
//    width = 2;
//    height = 2;
    vector<Vertex> * vertices = new vector<Vertex>();
    PerlinNoise noise(width + 1, height + 1);
    float ** pn = noise.perlinNoise(6);
    for (int i = 0; i < width + 1; i++) {
        for (int j = 0; j < height + 1; j++) {
            Vertex v((i - width/2) * 1, pn[i][j], (j - height/2) * 1);
            v.texCoords.x = (float)i /width;
            v.texCoords.y = (float)j/ height;
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
    string path1 = "Resources/Textures/grass_green_d.jpg";
    string path2 = "Resources/Textures/ground_dry2_d.jpg";
    string type = "texture_diffuse";
    textures->push_back(Texture(TextureFromFile(path1.c_str()), type));
    textures->push_back(Texture(TextureFromFile(path2.c_str()), type));
    mesh = new TexturedMesh(vertices, indices, textures);
//    glGenTextures(1, &textureA);
//    glGenTextures(1, &textureB);
    
}

void Landscape::render() {
    mesh->render(shader);
}
