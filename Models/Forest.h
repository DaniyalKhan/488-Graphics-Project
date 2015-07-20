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
#include "intersection.h"

class Forest {
private:
    GLuint shader;
    vector<glm::mat4> * positions;
    vector<glm::mat4> * shears;
    Mesh * m;
public:
    Model * tree;
    Forest(GLuint shader, vector<glm::mat4> * translations);
    void render();
    bool intersect(glm::vec3 pos, glm::vec3 dir, float &t);
    bool intersect(glm::vec3 b1, glm::vec3 b2, glm::mat4 transform);
    bool shake(glm::vec3 view, glm::vec3 pos, glm::vec3 &out);
};

#endif /* defined(__Project__Forest__) */
