//
//  World.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__World__
#define __Project__World__

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>
#include <ctime>
#include "player.h"
#include "Camera.h"
#include "Keyboard.h"
#include "ShaderManager.h"


class World {
public:
    World(GLFWwindow * window);
    void update();
    void render();
private:
    Player * player;
    Camera * camera;
    Keyboard * keyboard;
    ShaderManager manager;
    long double lastTime;
    glm::mat4 projectionMatrix;
//Shaders
private:
    static const int SHADER_MODEL = 0;
    GLuint bindShader(int shaderKey);
};

#endif /* defined(__Project__World__) */
