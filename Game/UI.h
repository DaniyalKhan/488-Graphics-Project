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
#include "TexturedMesh.h"
#include "Loader.h"

class UI {
private:
    //top, bottom, left, right
    TexturedMesh * meshFrom(glm::vec4 pos, glm::vec4 tex);
    vector<Texture> * t;
public:
    GLuint shader;
    UI(GLuint shader, float w, float h);
    TexturedMesh * crosshair;
    TexturedMesh * crosshairHit;
    void render(bool crosshair);
};

#endif /* defined(__Project__UI__) */
