//
//  Model.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-25.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Model__
#define __Project__Model__

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "Mesh.h"

using namespace std;

class Model {
public:
//private:
    vector<Mesh * > * meshes;
    Mesh * initMesh(const aiMesh * sceneMesh);
public:
    bool loadModel(const string& file);
    void render(GLuint shader);
};

#endif /* defined(__Project__Model__) */
