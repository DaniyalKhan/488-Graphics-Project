//
//  ShaderManager.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-29.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__ShaderManager__
#define __Project__ShaderManager__

#include <glew.h>
#include <OpenGL/gl3.h>
#include <string>
#include <map>

using namespace std;

class ShaderManager {
private:
    static const string SHADER_ROOT_PATH;
    map<int, GLuint> shaderPrograms;
public:
    ShaderManager();
    GLuint retrieveShader(int key);
    GLuint manageShader(int key, const string& directory);
};

#endif /* defined(__Project__ShaderManager__) */
