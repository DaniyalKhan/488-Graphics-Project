//
//  Model.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-25.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Model.h"
#include <iostream>

glm::mat4 Model::rotateX = glm::rotate(glm::mat4(),
                                           glm::radians(270.0f),
                                           glm::vec3(1.0f, 0.0f, 0.0f));

glm::mat4 Model::rotateY = glm::rotate(glm::mat4(),
                                           glm::radians(90.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f));

Model::Model(const string& path, bool transform) {
    this->path = path;
    size_t idx = path.find_last_of('/');
    directory = idx == string::npos ? "" : path.substr(0, path.find_last_of('/'));
    loadModel(path);
    if (transform) {
        transformMatrix = rotateY * rotateX;
    }
    min = glm::vec3(FLT_MAX);
    max = glm::vec3(FLT_MIN);
    for(vector<Mesh *>::iterator it = meshes->begin(); it != meshes->end(); ++it) {
        glm::vec3 meshMin = (*it)->min;
        glm::vec3 meshMax = (*it)->max;
        if (meshMin.x < min.x) min.x = meshMin.x;
        if (meshMax.x > max.x) max.x = meshMax.x;
        
        if (meshMin.y < min.y) min.y = meshMin.y;
        if (meshMax.y > max.y) max.y = meshMax.y;
        
        if (meshMin.z < min.z) min.z = meshMin.z;
        if (meshMax.z > max.z) max.z = meshMax.z;
    }
    
}

Model::Model(const string& path) : Model(path, true) {
    
}

void Model::rot(float degrees, glm::vec3 axis, glm::vec3 pos) {
    glm::mat4 trans = glm::translate(glm::mat4(), pos);
    trans = glm::rotate(trans, glm::radians(degrees), axis);
    trans = glm::translate(trans, -pos);
    transformMatrix = trans * transformMatrix;
}

void Model::rot(float degrees, glm::vec3 axis) {
    transformMatrix = glm::rotate(transformMatrix, glm::radians(degrees), axis);
}

glm::vec3 Model::position() {
    return glm::vec3(translationMatrix[3]);
}


void Model::render() {
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix()));
    for(vector<Mesh *>::iterator it = meshes->begin(); it != meshes->end(); ++it) {
        (*it)->render(shader);
    }
}

void Model::setShader(GLuint shader) {
    this->shader = shader;
}


glm::mat4 Model::modelMatrix() {
    return translationMatrix * transformMatrix;
}

void Model::translate(glm::vec3 trans) {
    translationMatrix = glm::translate(translationMatrix, trans);
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
        vector<Texture> * diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
        textures->insert(textures->end(), diffuseMaps->begin(), diffuseMaps->end());
        // 2. Specular maps
        vector<Texture> * specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
        textures->insert(textures->end(), specularMaps->begin(), specularMaps->end());
    }
    
    return new TexturedMesh(vertices, indices, textures);
}

void Model::applyColor(glm::vec3 color, int mesh) {
    Mesh * m = meshes->at(mesh);
    for (int i = 0; i < m->vertices->size(); i++) {
        m->vertices->at(i).color = color;
    }
    glBindVertexArray(m->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
    glBufferData(GL_ARRAY_BUFFER, m->vertices->size() * sizeof(Vertex), &(m->vertices->at(0)), GL_STATIC_DRAW);
    glBindVertexArray(m->vao);
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
vector<Texture> * Model::loadMaterialTextures(const aiMaterial* mat, aiTextureType type) {
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
            Texture texture(TextureFromFile(str.C_Str(), directory == "" ? NULL : directory.c_str()), str);
            textures->push_back(texture);
            // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            allTextures.push_back(texture);
        }
    }
    return textures;
}

void Model::scale(glm::vec3 scale) {
    transformMatrix = glm::scale(transformMatrix, scale);
}