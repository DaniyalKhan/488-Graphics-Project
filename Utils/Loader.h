//
//  Loader.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-24.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Loader__
#define __Project__Loader__

#include <stdio.h>
#include <string>
#include <iostream>
#include <SOIL.h>
#include "glew.h"
//#include <SFML/Audio.hpp>

typedef unsigned long DWORD;

using namespace std;

GLuint LoadShader(const char *vertex_path, const char *fragment_path);
GLuint TextureFromFile(const char* path, const char* directory);
GLuint TextureFromFilePNG(const char* path, const char* directory);

//void play();

#endif /* defined(__Project__Loader__) */

