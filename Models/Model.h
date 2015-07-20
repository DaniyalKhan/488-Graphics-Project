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
#include "TexturedMesh.h"
#include "Loader.h"
using namespace std;

class Model {
private:
    string directory;
    bool loadModel(const string& path);
    Mesh * initMesh(const aiMesh * sceneMesh, aiMaterial * const * meshMaterials);
    vector<Texture> * loadMaterialTextures(const aiMaterial* mat, aiTextureType type);
protected:
    static glm::mat4 rotateX;
    static glm::mat4 rotateY;
    glm::mat4 translationMatrix;
    glm::mat4 transformMatrix;
    glm::vec3 min, max;
    GLuint shader;
public:
    string path;
    vector<Mesh *> * meshes;
    Model(const string& path);
    Model(const string& path, bool transform);
    void render();
    void setShader(GLuint shader);
    void translate(glm::vec3 trans);
    void resetTranslation() {
        translationMatrix = glm::mat4();
    }
    glm::vec3 position();
    glm::vec3 getMin() { return min; }
    glm::vec3 getMax() { return max; }
    void scale(glm::vec3 scale);
    virtual glm::mat4 modelMatrix();
    void applyColor(glm::vec3 color, int mesh);
};

#endif /* defined(__Project__Model__) */
