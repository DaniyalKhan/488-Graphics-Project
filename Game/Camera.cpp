//
//  Camera.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#define GLM_SWIZZLE
#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 lookAt) {
    this->lookAt = glm::vec3(lookAt.x, lookAt.y, lookAt.z);
    float distance = 35.0f;
    float comp = distance/glm::sqrt(4.0f);
        this->position = glm::vec3(lookAt.x, lookAt.y + comp, lookAt.z + 3 * comp);
    up = glm::normalize(glm::cross(lookAt - position, glm::vec3(-1.0f, 0, 0)));
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, lookAt, up);;
}

void Camera::pan(float degrees) {
    glm::mat4 transform = glm::rotate(glm::radians(degrees), glm::vec3(0, 1, 0));
    transform = glm::translate(transform, -lookAt);
    position = (transform * glm::vec4(position, 1)).xyz();
    glm::vec3 v = glm::vec3(-position.x, 0, -position.z);
    glm::vec3 perp = glm::rotateY(v, glm::radians(90.0f));
    up = glm::normalize(glm::cross(-position, perp));
    position += lookAt;
}

void Camera::roll(float degrees) {
    glm::vec3 oldPosition = position;
    position -= lookAt;
    glm::vec3 perp = glm::cross(up, -position);
    glm::mat4 transform = glm::rotate(glm::mat4(), glm::radians(degrees), perp);
    position = (transform * glm::vec4(position, 1)).xyz();
    up = glm::normalize(glm::cross(-position, perp));
    float cosAnglePlaneXZ = glm::dot(up, glm::vec3(0, 1, 0));
    float cosanglePlaneY = glm::dot(up, glm::vec3(-position.x, 0, -position.z));
    std::cout<<glm::dot(up, glm::vec3(0, 1, 0))<<std::endl;
    if (cosanglePlaneY > 0 && cosAnglePlaneXZ > 0) {
        position += lookAt;
    } else {
        position = oldPosition;
    }
}