//
//  Character.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Character__
#define __Project__Character__

#include "Model.h"
#include "Animation.h"

class Character : public Model {
public:
    Character(const string& path, GLuint shader);
    Character(const string& path, GLuint shader, Animation * anim);
    glm::vec3 viewDirection();
    void strafe(float degrees);
    glm::vec3 forward(float distance, float delta);
    void addHeight(float height);
    void rot(float angle, glm::vec3 rot);
    glm::mat4 modelMatrix() {
        if (anim != NULL) {
            return translationMatrix * anim->getAnimation() * transformMatrix;
        } else return Model::modelMatrix();
    }
    Animation * anim;
protected:
    glm::vec3 view;
};

#endif /* defined(__Project__Character__) */
