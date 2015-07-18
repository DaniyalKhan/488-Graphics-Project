//
//  Player.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Player__
#define __Project__Player__

#include "Character.h"
#include <stack>

class Player : public Character {
public:
    Mesh * m;
    Player(const string& path, GLuint shader, Animation * anim);
    void render();
};

#endif /* defined(__Project__Player__) */
