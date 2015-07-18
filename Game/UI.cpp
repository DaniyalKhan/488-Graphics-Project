//
//  UI.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-18.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "UI.h"

static float width = 512;
static float height = 512;

UI::UI(GLuint s, float w, float h) : shader(s) {
    
    float chw = 0.05;
    float chh = 0.05 * w /h;
    
    const char * file = "uipackSpace_sheet.png";
    const char * directory = "Resources/Textures";
    t = new vector<Texture>();
    GLuint texture = TextureFromFilePNG(file, directory);
    t->push_back(Texture(texture));
    crosshair = meshFrom(glm::vec4(chh, -chh, -chw, chw), glm::vec4(300, 292, 36, 36));
    float chs = 0.8f;
    crosshairHit = meshFrom(glm::vec4(chh * chs, -chh* chs, -chw* chs, chw* chs), glm::vec4(300, 436, 32, 32));
}

void UI::render(bool c) {
    if (c) {
        crosshairHit->render(shader);
    } else {
        crosshair->render(shader);
    }
}

TexturedMesh * UI::meshFrom(glm::vec4 pos, glm::vec4 tex) {
    
    vector<Vertex> * vertices = new vector<Vertex>();
    vertices->push_back(Vertex(pos.w, pos.x, 0, 0, 0, 0, (tex.x + tex.z)/width, (tex.y + tex.w)/height));
    vertices->push_back(Vertex(pos.z, pos.x, 0, 0, 0, 0, tex.x/width, (tex.y + tex.w)/height));
    vertices->push_back(Vertex(pos.w, pos.y, 0, 0, 0, 0, (tex.x + tex.z)/width, tex.y/height));
    vertices->push_back(Vertex(pos.z, pos.y, 0, 0, 0, 0, tex.x/width, tex.y/height));
    
    vector<unsigned int> * indices = new vector<unsigned int>();
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(3);
    
    return new TexturedMesh(vertices, indices, t);
}
