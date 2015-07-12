//
//  Model.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-25.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Model.h"
#include <iostream>

Model::Model(const string& path) {
    directory = path.substr(0, path.find_last_of('/'));
    loadModel(path);
}

void Model::render() {
    for(vector<Mesh *>::iterator it = meshes->begin(); it != meshes->end(); ++it) {
        (*it)->render(shader);
    }
}

void Model::setShader(GLuint shader) {
    this->shader = shader;
}

bool Model::loadModel(const string& path) {
    Assimp::Importer Importer;
    const aiScene* pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (pScene) {
        meshes = new vector<Mesh *>();
        for (int i = 0; i < pScene->mNumMeshes; i++) {
            Mesh * m = initMesh(pScene->mMeshes[i], pScene->mMaterials);
            meshes->push_back(m);
        }
    } else {
        printf("Error parsing '%s': '%s'\n", path.c_str(), Importer.GetErrorString());
        return false;
    }
    return true;
}

Mesh * Model::initMesh(const aiMesh * sceneMesh,  aiMaterial * const * meshMaterials) {
    vector<Vertex> * vertices = new vector<Vertex>();
    vector<unsigned int> * indices = new vector<unsigned int>();
    vector<Texture> * textures = new vector<Texture>();
    
    for (int i = 0 ; i < sceneMesh->mNumVertices; i++) {
        const aiVector3D* pos = &(sceneMesh->mVertices[i]);
        const aiVector3D* norm = &(sceneMesh->mVertices[i]);
        const aiVector3D* tex = sceneMesh->mTextureCoords[0] ? &(sceneMesh->mTextureCoords[0][i]) : NULL;
        Vertex v(pos->x, pos->y, pos->z, norm->x, norm->y, norm->z, tex == NULL ? 0.0f : tex->x, tex == NULL ? 0.0f : tex->y);
        vertices->push_back(v);
    }
    
    for (int i = 0 ; i < sceneMesh->mNumFaces; i++) {
        const aiFace& Face = sceneMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indices->push_back(Face.mIndices[0]);
        indices->push_back(Face.mIndices[1]);
        indices->push_back(Face.mIndices[2]);
    }
    
    // Process materials
    if(sceneMesh->mMaterialIndex != INVALID_MATERIAL) {
        const aiMaterial * material = meshMaterials[sceneMesh->mMaterialIndex];
        // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // Diffuse: texture_diffuseN
        // Specular: texture_specularN
        // Normal: texture_normalN
        
        // 1. Diffuse maps
        vector<Texture> * diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures->insert(textures->end(), diffuseMaps->begin(), diffuseMaps->end());
        // 2. Specular maps
        vector<Texture> * specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures->insert(textures->end(), specularMaps->begin(), specularMaps->end());
    }
    
    return new TexturedMesh(vertices, indices, textures);
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
vector<Texture> * Model::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> allTextures;
    vector<Texture> * textures = new vector<Texture>();
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        GLboolean skip = false;
        for(GLuint j = 0; j < allTextures.size(); j++) {
            if(allTextures.at(j).path == str) {
                textures->push_back(allTextures.at(j));
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip) {   // If texture hasn't been loaded already, load it
            Texture texture(TextureFromFile(str.C_Str()), typeName, str);
            textures->push_back(texture);
            // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            allTextures.push_back(texture);
        }
    }
    return textures;
}