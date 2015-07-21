//
//  UI.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-18.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__UI__
#define __Project__UI__

#include <stdio.h>
#include <glew.h>
#include <GLFW/glfw3.h>
#include "TexturedMesh.h"
#include "Loader.h"
#include "pugixml.hpp"
#include <strstream>
#include "Model.h"

class UI {
private:
    //top, bottom, left, right
    TexturedMesh * meshFrom(glm::vec4 pos, glm::vec4 tex, vector<Texture> * texture);
    vector<Texture> * t;
    vector<Texture> * font;
    pugi::xml_document fontDoc;
    glm::vec4 fontTex[256];
    string path = "";
    Model * m;
    float scale;
    GLuint modelShader;
    void renderModel();
public:
    GLuint shader;
    UI(GLuint shader, float w, float h);
    TexturedMesh * crosshair;
    TexturedMesh * crosshairHit;
    TexturedMesh * panel;
    TexturedMesh * lastText = NULL;
    
    void renderCrossHair(bool crossHair);
    void render(string path, GLuint Modelshader);
    void setText(const char * text, float x, float y, bool centered);
    
    void interactModel(int key, float degrees);
};

#endif /* defined(__Project__UI__) */
