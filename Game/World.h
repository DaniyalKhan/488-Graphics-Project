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
#include <chrono>
#include <iterator>
#include <time.h>
#include "player.h"
#include "Camera.h"
#include "Keyboard.h"
#include "ShaderManager.h"
#include "Skybox.h"
#include "Landscape.h"
#include "Forest.h"

class World {
public:
    World(GLFWwindow * window);
    void update();
    void render();
private:
    Player * player;
    Camera * camera;
    Keyboard * keyboard;
    Skybox * skybox;
    ShaderManager manager;
    unsigned long lastTime;
    glm::mat4 projectionMatrix;
    Forest * forest;
    Landscape * landscape;
    float extra;
    void moveCharacter(float direction);
//Shaders
private:
    static const int SHADER_TEXTURED_MODEL = 0;
    static const int SHADER_GROUND = 1;
    static const int SHADER_SKY = 2;
    static const int SHADER_TREE = 3;
    GLuint bindShader(int shaderKey);
};

#endif /* defined(__Project__World__) */
