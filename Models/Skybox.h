//
//  Skybox.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-01.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Skybox__
#define __Project__Skybox__

#include <glew.h>
#include <OpenGL/gl3.h>
#include <SOIL.h>
#include <vector>
#include "Mesh.h"

class Skybox {
private:
    Mesh * mesh;
    GLuint textureID;
    GLuint shader;
public:
    Skybox(std::vector<const char *> faces, GLuint shader);
    void render();
private:
    static float skyboxVertices[108];
};

#endif /* defined(__Project__Skybox__) */
