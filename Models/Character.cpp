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
    view = glm::vec3(0, 0, -1);
    this->anim = NULL;
}

Character::Character(const string& path, GLuint shader, Animation * anim) : Model(path) {
    setShader(shader);
    view = glm::vec3(0, 0, -1);
    this->anim = anim;
}

glm::vec3 Character::position() {
    return glm::vec3(translationMatrix[3]);
}

glm::vec3 Character::viewDirection() {
    return view;
//    return (transformMatrix * glm::vec4(view, 0)).xyz();
}

void Character::strafe(float degrees) {
    transformMatrix = glm::rotate(transformMatrix, glm::radians(degrees), glm::vec3(0, 0, 1));
    view = (glm::rotate(glm::mat4(), glm::radians(degrees), glm::vec3(0, 1, 0)) * glm::vec4(view, 1)).xyz();
}

void Character::addHeight(float height) {
    translate(glm::vec3(0, height, 0));
}

glm::vec3 Character::forward(float distance, float delta) {
    glm::vec3 trans = view * distance;
    translate(trans);
    if (anim != NULL) {
        anim->animate(delta);
    }
    return trans;
}