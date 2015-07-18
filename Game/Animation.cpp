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

JumpAnimation::JumpAnimation()  {
    
}

void JumpAnimation::animate(float delta) {
    time += delta;
    height = abs(sin(time) * 2);
    transform = translate(glm::mat4(), vec3(0,height,0));
}