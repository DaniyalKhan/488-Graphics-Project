//
//  Landscape.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-11.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Landscape__
#define __Project__Landscape__

#include <math.h>
#include "TexturedMesh.h"
#include "PerlinNoise.h"
#include "Loader.h"

class Landscape {
private:
    TexturedMesh * mesh;
    int width, height;
    float scale;
    GLuint shader;
    float epsilon = 0.05f;
public:
    Landscape(int width, int height, float scale, GLuint shader);
    glm::vec3 positionAt(float x, float z);
    void render();
};

#endif /* defined(__Project__Landscape__) */
