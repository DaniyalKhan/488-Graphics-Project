//
//  Forest.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-13.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Forest__
#define __Project__Forest__

#include <stdio.h>
#include <glew.h>
#include <OpenGL/gl3.h>
#include <vector>
#include "Model.h"
//#include "intersection.h"

class Forest {
private:
    GLuint shader;
    Model * tree;
    vector<glm::mat4> * positions;
    Mesh * m;
public:
    Forest(GLuint shader, vector<glm::mat4> * translations);
    void render();
    bool intersect(glm::vec3 b1, glm::vec3 b2, glm::mat4 transform);
};

#endif /* defined(__Project__Forest__) */
