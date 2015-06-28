//
//  Player.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Player.h"

Player::Player(const string& path, GLFWwindow * window) : Character(path) {
    this->window = window;
}

void Player::setup(glm::vec3 position, glm::vec3 lookAt) {
    this->position = position;
    camera.position = position;
    
    camera.lookAt = lookAt;
}