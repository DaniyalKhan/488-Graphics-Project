//
//  TexturedMesh.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-01.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__TexturedMesh__
#define __Project__TexturedMesh__

#include "Mesh.h"
#include <sstream>

class TexturedMesh : public Mesh {
private:
    vector<Texture> * textures;
public:
    TexturedMesh(vector<Vertex> * v, vector<unsigned int> * i, vector<Texture> * t);
    void render(GLuint shader);
};

#endif /* defined(__Project__TexturedMesh__) */
