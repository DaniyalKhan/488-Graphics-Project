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
#include <algorithm>
#include <map>
#include "player.h"
#include "Camera.h"
#include "Keyboard.h"
#include "ShaderManager.h"
#include "Skybox.h"
#include "Landscape.h"
#include "Forest.h"
#include "UI.h"
#include "intersection.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_common.h"
#include "fmod_dsp_effects.h"
#include "fmod_output.h"
#include "fmod_common.h"

class World {
public:
    World(GLFWwindow * window);
    void update();
    void render();
private:
    int width, height;
    Player * player;
    int curPlayer = 0;
    Camera * camera;
    bool firstPerson = false;
    bool modelMode = false;
    Keyboard * keyboard;
    Skybox * skybox;
    ShaderManager manager;
    unsigned long lastTime;
    glm::mat4 projectionMatrix;
    Forest * forest;
    Landscape * landscape;
    Model * grassA;
    vector<glm::vec3> * grassPositions;
    vector<glm::vec3> * grassSizes;
    float extra;
    void moveCharacter(float direction, float delta);
    UI * ui;
    vector<Character * > * jumpers;
    vector<Character * > * flyers;
    vector<glm::vec3> * rotations;
    vector<Character * > * fallers;
    vector<Character *> * characters;
    vector<Character *> * standers;
    
    int cut; int maxCut;
    int seen;
    int watered;
    
    bool reflect = false;
    
    vector<string> pokemonSeen;
    vector<bool> flowersWatered;
    
    FMOD::System *system;
    
    map<string, FMOD::Sound *> cries;
    FMOD::Channel * channel2 = 0;
    FMOD::Channel * channel3 = 0;
    
    FMOD::Sound * collisionSound;
    FMOD::Sound * cutSound;
    FMOD::Sound * waterSound;
    FMOD::Sound * fallSound;
    string lastPath;
    
//Shaders
private:
    static const int SHADER_TEXTURED_MODEL = 0;
    static const int SHADER_GROUND = 1;
    static const int SHADER_SKY = 2;
    static const int SHADER_TREE = 3;
    static const int SHADER_UI = 4;
    static const int SHADER_FONT = 5;
    static const int SHADER_REFLECTION = 6;
    GLuint bindShader(int shaderKey);
};

#endif /* defined(__Project__World__) */
