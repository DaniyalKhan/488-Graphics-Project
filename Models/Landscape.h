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
    bool outofBounds(float x, float z) {
        return (abs(x) + 5) >= scale * width/2 || (abs(z) + 5) >= scale * height/2;
//        if (x <= -scale * width/2) x = -scale * width/2 + 1;
//        if (x >= scale * width/2) x = scale * width/2 + 1;
//        if (z <= -scale * height/2) x = -scale * height/2 + 1;
//        if (z >= scale * height/2) x = scale * height/2 + 1;
    }
    
};

#endif /* defined(__Project__Landscape__) */
