//
//  Landscape.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-11.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Landscape__
#define __Project__Landscape__

#include "TexturedMesh.h"
#include "PerlinNoise.h"
#include "Loader.h"

class Landscape {
private:
    TexturedMesh * mesh;
    int width, height;
    GLuint shader;
//    GLuint textureA;
//    GLuint textureB;
public:
    Landscape(int width, int height, GLuint shader);
    void render();
};

#endif /* defined(__Project__Landscape__) */
