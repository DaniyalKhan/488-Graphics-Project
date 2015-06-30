//
//  Character.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Character__
#define __Project__Character__

#include "Model.h"


class Character {
public:
    Character(const string& path);
    void render(GLuint shader);
    void setPosition(glm::vec3 position);
    glm::mat4 getModelMatrix();
protected:
    static glm::mat4 rotateX;
    static glm::mat4 rotateY;
    Model * model;
    glm::vec3 position;
    glm::mat4 modelMatrix;
};

#endif /* defined(__Project__Character__) */
