//
//  Character.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#define GLM_SWIZZLE
#include "Character.h"


Character::Character(const string& path, GLuint shader) : Model(path) {
    setShader(shader);
    view = glm::vec3(1, 0, 0);
}

glm::vec3 Character::position() {
    return glm::vec3(translationMatrix[3]);
}

glm::vec3 Character::viewDirection() {
    return (rotationMatrix * glm::vec4(view, 0)).xyz();
}
