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


class Character : public Model{
public:
    Character(const string& path, GLuint shader);
    glm::vec3 viewDirection();
    glm::vec3 position();
protected:
    glm::vec3 view;
};

#endif /* defined(__Project__Character__) */
