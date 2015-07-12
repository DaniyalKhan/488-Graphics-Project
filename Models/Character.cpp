//
//  Character.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#define GLM_SWIZZLE
#include "Character.h"

glm::mat4 Character::rotateX = glm::rotate(glm::mat4(),
                                           glm::radians(270.0f),
                                           glm::vec3(1.0f, 0.0f, 0.0f));

glm::mat4 Character::rotateY = glm::rotate(glm::mat4(),
                                           glm::radians(90.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f));

Character::Character(const string& path, GLuint shader) {
    model = new Model(path);
    model->setShader(shader);
    rotationMatrix = rotateY * rotateX;
    view = glm::vec3(1, 0, 0);
}

void Character::render() {
    model->render();
}

glm::mat4 Character::modelMatrix() {
    return translationMatrix * rotationMatrix;
}

void Character::translate(glm::vec3 position) {
    translationMatrix = glm::translate(translationMatrix, position);
}

glm::vec3 Character::position() {
    return glm::vec3(translationMatrix[3]);
}

glm::vec3 Character::viewDirection() {
    return (rotationMatrix * glm::vec4(view, 0)).xyz();
}
