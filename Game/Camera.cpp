//
//  Camera.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 lookAt) {
    this->lookAt = lookAt;
    float distance = 30.0f;
    float comp = distance/glm::sqrt(4.0f);
    this->position = glm::vec3(lookAt.x, lookAt.y + comp, lookAt.z + 3 * comp);
    this->up = glm::cross(lookAt - position, glm::vec3(-1.0f, 0, 0));
}

glm::mat4 Camera::getViewMatrix() {
        std::cout << glm::to_string(glm::lookAt(position, lookAt, up)) << std::endl;
    return glm::lookAt(position, lookAt, up);
}