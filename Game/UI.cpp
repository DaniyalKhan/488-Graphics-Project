//
//  UI.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-18.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "UI.h"
#include <iostream>

static float width = 512;
static float height = 512;

static float widthFont = 256;
static float heightFont = 128;

UI::UI(GLuint s, float w, float h) : shader(s) {
    
    const char * file = "uipackSpace_sheet.png";
    const char * directory = "Resources/Textures";
    t = new vector<Texture>();
    GLuint texture = TextureFromFilePNG(file, directory);
    t->push_back(Texture(texture));
    
    const char * fontFile = "courier_new_bold_14.PNG";
    const char * fontDirectory = "Resources/Fonts";
    font = new vector<Texture>();
    GLuint textureFont = TextureFromFilePNG(fontFile, fontDirectory);
    font->push_back(Texture(textureFont));
    
    pugi::xml_parse_result result = fontDoc.load_file("Resources/Fonts/courier_new_bold_14.xml");
    std::cout << "Load result: " << result.description() <<endl;
    pugi::xml_node glyphs = fontDoc.child("Font");
    for (pugi::xml_node_iterator it = glyphs.begin(); it != glyphs.end(); ++it) {
        int c = it->attribute("code").value()[0];
        stringstream ss(it->attribute("rect").value());
        glm::vec4 tex;
        std::string token;
        std::getline(ss, token, ' '); tex.x = atoi(token.c_str())/widthFont;
        std::getline(ss, token, ' '); tex.y = atoi(token.c_str())/heightFont;
        std::getline(ss, token, ' '); tex.z = atoi(token.c_str())/widthFont;
        std::getline(ss, token, ' '); tex.w = atoi(token.c_str())/heightFont;
        tex.y += tex.w;
        tex.w = -tex.w;
        fontTex[c] = tex;
    }
    
    float chw = 0.03;
    float chh = 0.03 * w /h;
    
    crosshair = meshFrom(glm::vec4(chh, -chh, -chw, chw), glm::vec4(332/width, 468/height, 32/width, 32/height), t);
    float chs = 1.0f;
    crosshairHit = meshFrom(glm::vec4(chh * chs, -chh* chs, -chw* chs, chw* chs), glm::vec4(300/width, 436/height, 32/width, 32/height), t);
    
    panel = meshFrom(glm::vec4(-0.2* w /h, -0.5* w /h, 0.6, 0.9), glm::vec4(100/width, 200/height, 100/width, 100/height), t);
}

void UI::renderModel() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(modelShader);
    glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0.75, -0.6, 0));
    glUniformMatrix4fv(glGetUniformLocation(modelShader, "projection"), 1, GL_FALSE, value_ptr(trans * glm::perspective(glm::radians(60.0f), ((float)1280)/720, 0.01f, 1000.0f)));

    glm::vec3 min = m->getMin() * scale;
    glm::vec3 max = m->getMax() * scale;
    float y = (min.z + max.z)/2 + fmin(min.z, max.z);

    glm::vec3 viewPos = glm::vec3(0, y, -1);
    
    glUniform3f(glGetUniformLocation(modelShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(modelShader, "lightPos"), viewPos.x, viewPos.y + 0.1f, viewPos.z);

    
    glUniformMatrix4fv(glGetUniformLocation(modelShader, "view"), 1, GL_FALSE, value_ptr(glm::lookAt(viewPos, glm::vec3(0, y, 0), glm::vec3(0, 1, 0))));
    
    m->render();
}

void UI::render(bool c, string path, GLuint mShader) {
    if (c) {
        crosshairHit->render(shader);
    } else {
        crosshair->render(shader);
    }
    panel->render(shader);
    
    if (lastText != NULL) {
        lastText->render(shader);
    }
    
    if (path != "" && this->path != path ) {
        m = new Model(path);
        float maxZ = fmax(m->getMin().z, m->getMax().z);
        
        scale = 1.0f/maxZ * 0.2f;
        m->scale(glm::vec3(scale));
        this->modelShader = mShader;
        m->setShader(modelShader);
        this->path = path;
    }
    if (this->path != "") {
        renderModel();
    }

}

void UI::setText(const char * text, float x, float y, bool centered) {
    if (text[0] == '\0') {
        lastText = NULL;
        return;
    }
    vector<Vertex> * vertices = new vector<Vertex>();
    vector<unsigned int> * indices = new vector<unsigned int>();
    float xSum = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        glm::vec4 t = fontTex[text[i]];
        xSum += t.z + 0.01;
    }
    x -= xSum/2;
    for (int i = 0; text[i] != '\0'; i++) {
        glm::vec4 tex = fontTex[text[i]];
        vertices->push_back(Vertex(x + tex.z, y - tex.w, 0, 0, 0, 0, (tex.x + tex.z), (tex.y + tex.w)));
        vertices->push_back(Vertex(x, y - tex.w, 0, 0, 0, 0, tex.x, (tex.y + tex.w)));
        vertices->push_back(Vertex(x + tex.z, y, 0, 0, 0, 0, (tex.x + tex.z), tex.y));
        vertices->push_back(Vertex(x, y, 0, 0, 0, 0, tex.x, tex.y));
        x += tex.z + 0.01;
        indices->push_back(4*i);
        indices->push_back(4*i + 1);
        indices->push_back(4*i + 2);
        indices->push_back(4*i + 1);
        indices->push_back(4*i + 2);
        indices->push_back(4*i + 3);
    }
    
    lastText = new TexturedMesh(vertices, indices, font);
    
}

TexturedMesh * UI::meshFrom(glm::vec4 pos, glm::vec4 tex, vector<Texture> * texture) {
    
    vector<Vertex> * vertices = new vector<Vertex>();
    vertices->push_back(Vertex(pos.w, pos.x, 0, 0, 0, 0, (tex.x + tex.z), (tex.y + tex.w)));
    vertices->push_back(Vertex(pos.z, pos.x, 0, 0, 0, 0, tex.x, (tex.y + tex.w)));
    vertices->push_back(Vertex(pos.w, pos.y, 0, 0, 0, 0, (tex.x + tex.z), tex.y));
    vertices->push_back(Vertex(pos.z, pos.y, 0, 0, 0, 0, tex.x, tex.y));
    
    vector<unsigned int> * indices = new vector<unsigned int>();
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(1);
    indices->push_back(2);
    indices->push_back(3);
    
    return new TexturedMesh(vertices, indices, texture);
}
