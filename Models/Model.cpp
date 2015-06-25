//
//  Model.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-25.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Model.h"
#include <iostream>

void Model::render(GLuint shader) {
    for(vector<Mesh *>::iterator it = meshes->begin(); it != meshes->end(); ++it) {
        (*it)->render(shader);
        std::cout << "rendering mesh" << endl;
    }
}

bool Model::loadModel(const string& file) {
    Assimp::Importer Importer;
    const aiScene* pScene = Importer.ReadFile(file.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (pScene) {
        meshes = new vector<Mesh *>();
        for (int i = 0; i < pScene->mNumMeshes; i++) {
            Mesh * m = initMesh(pScene->mMeshes[i]);
            meshes->push_back(m);
        }
    } else {
        printf("Error parsing '%s': '%s'\n", file.c_str(), Importer.GetErrorString());
        return false;
    }
    return true;
}

Mesh * Model::initMesh(const aiMesh * sceneMesh) {
    float * vertices = new float[sceneMesh->mNumVertices * 3];
    unsigned int * indices = new unsigned int[sceneMesh->mNumFaces * 3];
    
    for (int i = 0 ; i < sceneMesh->mNumVertices; i++) {
        const aiVector3D* pos = &(sceneMesh->mVertices[i]);
//        vertices->push_back(pos->x);
//        vertices->push_back(pos->y);
//        vertices->push_back(pos->z);
        vertices[3 * i] = pos->x;
        vertices[3 * i + 1] = pos->y;
        vertices[3 * i + 2] = pos->z;
    }
    
    for (int i = 0 ; i < sceneMesh->mNumFaces; i++) {
        const aiFace& Face = sceneMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
//        indices->push_back(Face.mIndices[0]);
//        indices->push_back(Face.mIndices[1]);
//        indices->push_back(Face.mIndices[2]);
        indices[3 * i] = Face.mIndices[0];
        indices[3 * i + 1] = Face.mIndices[1];
        indices[3 * i + 2] = Face.mIndices[2];
    }
    return new Mesh(vertices, sceneMesh->mNumVertices * 3, indices, sceneMesh->mNumFaces*3, NULL, 0);
}