//
//  Keyboard.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Keyboard.h"

using namespace std;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Keyboard * keyboard = reinterpret_cast<Keyboard *>(glfwGetWindowUserPointer(window));
    
    if (action == GLFW_PRESS) {
        keyboard->push(key);
    }
    else if (action == GLFW_RELEASE) {
        keyboard->remove(key);
    }
}

int * Keyboard::queryInput() {
    if (inputs.size() > 0) {
        return &inputs.back();
    }
    return NULL;
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