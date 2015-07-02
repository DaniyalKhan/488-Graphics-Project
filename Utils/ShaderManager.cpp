//
//  ShaderManager.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-29.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "ShaderManager.h"
#include "Loader.h"

using namespace std;

const string ShaderManager::SHADER_ROOT_PATH = "Shaders/";


ShaderManager::ShaderManager() {
    
}


GLuint ShaderManager::manageShader(int key, const string& directory) {
    string vertexShader, fragmentShader;
    
    vertexShader.assign(SHADER_ROOT_PATH + directory + ".vert");
    fragmentShader.assign(SHADER_ROOT_PATH + directory + ".frag");
    GLuint shader = LoadShader(vertexShader.c_str(), fragmentShader.c_str());
    shaderPrograms[key] = shader;
    return shader;
}

GLuint ShaderManager::retrieveShader(int key) {
    return shaderPrograms[key];
}
