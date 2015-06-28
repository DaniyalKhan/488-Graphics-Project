//
//  Character.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Character.h"

glm::mat4 Character::rotateX = glm::rotate(glm::mat4(),
                                           glm::radians(270.0f),
                                           glm::vec3(1.0f, 0.0f, 0.0f));

glm::mat4 Character::rotateY = glm::rotate(glm::mat4(),
                                           glm::radians(90.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f));


Character::Character(const string& path) {
    model = new Model(path);
    modelMatrix = rotateY * rotateX;
}

void Character::render(GLuint shader) {
    model->render(shader);
}

glm::mat4 Character::getModelMatrix() {
    return modelMatrix;
}
