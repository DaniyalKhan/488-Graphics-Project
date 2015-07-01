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

using namespace glm;

Camera::Camera(vec3 lookAt) {
    look(lookAt);
}

void Camera::look(vec3 lookAt) {
    float distance = 35.0f;
    float comp = distance/sqrt(4.0f);
    vec3 position = vec3(lookAt.x, lookAt.y + comp, lookAt.z + 3 * comp);
    this->look(lookAt, position);
}

void Camera::look(vec3 lookAt, vec3 position) {
    this->lookAt = lookAt;
    this->position = position;
    up = normalize(cross(lookAt - position, vec3(-1.0f, 0, 0)));
}

mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, lookAt, up);
}

void Camera::pan(float degrees) {
    mat4 transform = rotate(radians(degrees), vec3(0, 1, 0));
    transform = translate(transform, -lookAt);
    position = (transform * vec4(position, 1)).xyz();
    vec3 v = vec3(-position.x, 0, -position.z);
    vec3 perp = rotateY(v, radians(90.0f));
    up = normalize(cross(-position, perp));
    position += lookAt;
}

void Camera::roll(float degrees) {
    vec3 oldPosition = position;
    position -= lookAt;
    vec3 perp = cross(up, -position);
    mat4 transform = rotate(mat4(), radians(degrees), perp);
    position = (transform * vec4(position, 1)).xyz();
    up = normalize(cross(-position, perp));
    float cosAnglePlaneXZ = dot(up, vec3(0, 1, 0));
    float cosAnglePlaneY = dot(up, vec3(-position.x, 0, -position.z));
    if (cosAnglePlaneY > 0 && cosAnglePlaneXZ > 0) {
        position += lookAt;
    } else {
        position = oldPosition;
    }
}