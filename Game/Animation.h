//
//  Animation.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-16.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Animation__
#define __Project__Animation__

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/ext.hpp"

using namespace glm;

class Animation {
protected:
    mat4 transform;
    Animation() {};
public:
    mat4 getAnimation() {
        return transform;
    };
    virtual void animate(float delta) = 0;
};

class RotationAnimation : public Animation {
public:
    RotationAnimation(float time);
    void animate(float delta);
private:
    int t1 = -10;
    int t2 = 10;
    float time;
    float angle = 0;
};

class JumpAnimation : public Animation {
public:
    JumpAnimation();
    void animate(float delta);
private:
    int t1 = 0;
    int t2 = 1;
    float time;
    float height;
};

#endif /* defined(__Project__Animation__) */
