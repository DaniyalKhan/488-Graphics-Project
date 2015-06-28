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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/ext.hpp"


class Character {
public:
    Character(const string& path);
protected:
    static glm::mat4 rotateX;
    static glm::mat4 rotateY;
    Model * model;
    glm::vec3 position;
    glm::mat4 modelMatrix;
};

#endif /* defined(__Project__Character__) */
