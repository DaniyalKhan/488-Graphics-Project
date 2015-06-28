//
//  Player.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-27.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Player.h"

float Camera::distance = 30.0f;

glm::mat4 Player::getViewMatrix() {
    return glm::lookAt(camera.position, camera.lookAt, camera.up);
}

Player::Player(const string& path, GLFWwindow * window) : Character(path) {
    this->window = window;
    this->inputs = new stack<int>();
}

void Player::setup(glm::vec3 position) {
    this->position = position;
    float comp = Camera::distance/glm::sqrt(4.0f);
    camera.position = glm::vec3(position.x, position.y + comp, position.z + 3 * comp);
    camera.lookAt = position;
    camera.up = glm::cross(camera.lookAt - camera.position, glm::vec3(-1.0f, 0, 0));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        
    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        
    }
}