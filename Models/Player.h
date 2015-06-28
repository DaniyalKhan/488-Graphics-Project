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

struct Camera {
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};

class Player : public Character {
public:
    Player(const string& path, GLFWwindow * window);
    void setup(glm::vec3 position, glm::vec3 lookAt);
private:
    GLFWwindow * window;
    Camera camera;
};

#endif /* defined(__Project__Player__) */
