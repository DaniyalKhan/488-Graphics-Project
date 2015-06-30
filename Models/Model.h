//
//  Model.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-25.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Model__
#define __Project__Model__

#define INVALID_MATERIAL 0xFFFFFFFF

#include <glew.h>
#include <OpenGL/gl3.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/ext.hpp"
#include "Mesh.h"

using namespace std;

class Model {
private:
    vector<Mesh *> * meshes;
    string directory;
    //TODO should probably make this static, but why would different models use the same texture?
    vector<Texture> * allTextures;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    bool loadModel(const string& path);
    Mesh * initMesh(const aiMesh * sceneMesh, aiMaterial * const * meshMaterials);
public:
    Model(const string& path);
    void render(GLuint shader);
    vector<Texture> * loadMaterialTextures(const aiMaterial* mat, aiTextureType type, string typeName);
    GLint TextureFromFile(const char* path);
};

#endif /* defined(__Project__Model__) */
