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
#include <SOIL.h>
#include "glew.h"

GLuint LoadShader(const char *vertex_path, const char *fragment_path);
GLuint TextureFromFile(const char* path);

#endif /* defined(__Project__Loader__) */

