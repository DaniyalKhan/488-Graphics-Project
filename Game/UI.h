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
#include <map>
#include "TexturedMesh.h"
#include "Loader.h"
#include "pugixml.hpp"
#include <strstream>
#include "Model.h"
#include "ShaderManager.h"

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
    GLuint fontShader;
    UI(GLuint shader, float w, float h, GLuint fontShader);
    TexturedMesh * crosshair;
    TexturedMesh * crosshairHit;
    TexturedMesh * panel;
    TexturedMesh * metalPanel;
    TexturedMesh * metalPanel2;
    TexturedMesh * name = NULL;
    TexturedMesh * seen = NULL;
    TexturedMesh * cuts = NULL;
    TexturedMesh * watered = NULL;
    
    TexturedMesh * tag1 = NULL;
    TexturedMesh * tag2 = NULL;
    
    map<const char *, TexturedMesh *> texts;
    
    void renderCrossHair(bool crossHair);
    void render(string path, GLuint Modelshader);
    void setName(const char * n) {
        setText(n, &name, n, -0.65, -0.45, true);
    }
    void setCuts(int c, int total) {
        setText("T", &cuts, ("Trees Cut: " + getString(c) + "/" + getString(total)).c_str(), -0.875, -0.6, false);
    }
    void setSeen(int s, int total) {
        setText("S", &seen, ("Pokemon Seen: " + getString(s) + "/" + getString(total)).c_str(), -0.875, -0.7, false);
    }
    void setWatered(int w, int total) {
        setText("P", &watered, ("Plants Watered: " + getString(w) + "/" + getString(total)).c_str(), -0.875, -0.8, false);
    }
    
    void setTags(char * t1, char * t2) {
        setText(t1, &tag1, t1, 0.75, -0.17, true);
        setText(t2, &tag2, t2, 0.75, -0.23, true);
    }
    
    void setText(const char * kay, TexturedMesh **m, const char * text, float x, float y, bool centered);
    
    string getString(int i) {
        stringstream ss;
        ss << i;
        return ss.str();
    }
    
    void interactModel(int key, float degrees);
};

#endif /* defined(__Project__UI__) */
