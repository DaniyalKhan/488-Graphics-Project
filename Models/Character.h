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
    glm::vec3 position();
    void strafe(float degrees);
    glm::vec3 forward(float distance, float delta);
    void addHeight(float height);
    glm::mat4 modelMatrix() {
        if (anim != NULL) {
            return translationMatrix * anim->getAnimation() * transformMatrix;
        } else return Model::modelMatrix();
//        return Model::modelMatrix();
    }
protected:
    glm::vec3 view;
    Animation * anim;
};

#endif /* defined(__Project__Character__) */
