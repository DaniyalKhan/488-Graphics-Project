//
//  Keyboard.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Keyboard__
#define __Project__Keyboard__

#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

class Keyboard {
private:
    GLFWwindow * window;
    vector<int> inputs;
public:
    int * queryInput();
    Keyboard(GLFWwindow * window);
    void push(int input);
    void remove(int input);
};

#endif /* defined(__Project__Keyboard__) */
