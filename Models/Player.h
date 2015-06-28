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
#include <GLFW/glfw3.h>
#include <stack>

struct Camera {
    static float distance;
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;
};

class Player : public Character {
public:
    Player(const string& path, GLFWwindow * window);
    void setup(glm::vec3 position);
    glm::mat4 getViewMatrix();
private:
    GLFWwindow * window;
    std::stack<int> * inputs;
    Camera camera;
};

#endif /* defined(__Project__Player__) */
