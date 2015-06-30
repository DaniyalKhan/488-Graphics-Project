//
//  Keyboard.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Keyboard.h"
#include <algorithm>

using namespace std;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Keyboard * keyboard = reinterpret_cast<Keyboard *>(glfwGetWindowUserPointer(window));
    
    if (action == GLFW_PRESS) {
        keyboard->push(key);
    } else if (action == GLFW_RELEASE) {
        keyboard->remove(key);
    }
//    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
//        
//    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
//        
//    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
//        
//    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
//        
//    }
}

void Keyboard::push(int input) {
    inputs.push_back(input);
}

void Keyboard::remove(int input) {
    vector<int>::iterator it = find(inputs.begin(), inputs.end(), input);
    if (it != inputs.end()) {
        inputs.erase(it);
    }
}

Keyboard::Keyboard(GLFWwindow * w) : window(w) {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(w, keyCallback);
}