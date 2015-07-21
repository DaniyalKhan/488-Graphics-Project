//
//  Animation.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-16.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Animation.h"

RotationAnimation::RotationAnimation(float t) : time(t) {
    
}

void RotationAnimation::animate(float delta) {
    angle += (t2 - t1) * (delta / time);
    if (angle < -10 || angle > 10) {
        angle = fmin(fmax(-10, angle), 10);
        t2 ^= t1;
        t1 ^= t2;
        t2 ^= t1;
    }
    transform = rotate(glm::mat4(), glm::radians(angle), vec3(0,1,0));
}

JumpAnimation::JumpAnimation() {
    
}

void JumpAnimation::animate(float delta) {
    time += delta;
    height = abs(sin(time) * 3);
    transform = translate(glm::mat4(), vec3(0,height,0));
}

FlyAnimation::FlyAnimation(float v, glm::vec3 o) : vel(v), origin(o) {
    
}

void FlyAnimation::animate(float delta) {
    angle += vel * delta/1000;
    transform = translate(glm::mat4(), origin);
    transform = rotate(transform, glm::radians(angle), glm::vec3(0,1,0));
    transform = translate(transform, -origin);
}

FallAnimation::FallAnimation(glm::vec3 r, float s, float h) : speed(s), dir(r), height(h) {
    
}

void FallAnimation::animate(float delta) {
    time += delta/1000;
    glm::vec3 trans = glm::vec3(2 * speed * dir.x, dir.y * (-4.9 * time + speed), 2 * speed * dir.z) * time;
    if (trans.y < -height) {
        trans.y = -height;
        time -= delta/1000;
        trans = glm::vec3(2 * speed * dir.x, dir.y * (-4.9 * time + speed), 2 * speed * dir.z) * time;
    }
    transform = translate(glm::mat4(), trans);
}

RollAnimation::RollAnimation() {
    
}

void RollAnimation::animate(float delta) {
    angle += delta/10;
    transform = translate(glm::mat4(), rot);
    transform = rotate(transform, glm::radians(angle), glm::vec3(0,1,0));
    transform = translate(transform, -rot);
}