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
    void reset() {
        time = 0;
    }
private:
    int t1 = 0;
    int t2 = 1;
    float time;
    float height;
};

class FlyAnimation : public Animation {
public:
    FlyAnimation(float vel, glm::vec3 origin);
    void animate(float delta);
private:
    float angle = 0;
    float vel;
    glm::vec3 origin;
};

class FallAnimation : public Animation {
public:
    FallAnimation(glm::vec3 dir, float speed, float height);
    void animate(float delta);
private:
    glm::vec3 dir;
    float speed;
    float height;
    float time;
};

#endif /* defined(__Project__Animation__) */
