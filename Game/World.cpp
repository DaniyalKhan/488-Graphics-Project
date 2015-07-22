//
//  World.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#define GLM_SWIZZLE
#include <glew.h>
#include <iostream>
#include "World.h"


void FMODErrorCheck(FMOD_RESULT result)
{
    if (result != FMOD_OK) {
        std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
//        exit(-1);
    }
}


World::World(GLFWwindow * window) {
    keyboard = new Keyboard(window);
    glfwGetWindowSize(window, &width, &height);
    projectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width)/height, 0.01f, 1000.0f);
    
    int terrainWidth = 200;
    int terrainHeight = 200;
    int scale = 5;
    landscape = new Landscape(terrainWidth, terrainHeight, scale, manager.manageShader(SHADER_GROUND, "ModelShaders/Ground"));
    
    GLuint textureModelShader = manager.manageShader(SHADER_TEXTURED_MODEL, "ModelShaders/SimpleModel");
    GLuint treeShader = manager.manageShader(SHADER_TREE, "ModelShaders/Tree");

    glm::vec3 startingPosition = landscape->positionAt(0, 0);
    player = new Player("Resources/Models/Bulbasaur/Bulbasaur.dae", textureModelShader, new RotationAnimation(500.0f));

    player->translate(startingPosition);
    camera = new Camera(startingPosition);
    
    grassPositions = new vector<glm::vec3>();
    grassSizes = new vector<glm::vec3>();
    
    //grass 4, 6
    grassA = new Model("Resources/Models/Flower/Cloud Flower.obj", false);
    grassA->scale(glm::vec3(0.05, 0.05, 0.05));
    grassA->setShader(textureModelShader);
    
    vector<glm::mat4> * trees = new vector<glm::mat4>();
    srand (time(NULL));
    
    int numTrees = 75;
    for (int i = 0; i < numTrees; i++) {
        float x = ((rand() % terrainWidth) - terrainWidth/2) * scale * 0.75;
        float z = ((rand() % terrainHeight) - terrainHeight/2) * scale * 0.75;
        for (int j = 0; j < 2; j++) {
            glm::vec3 localTrans = glm::vec3(15 + rand() % 5, 0, 15 + rand() % 5);
//            localTrans = glm:;vec3();
            float angle = glm::radians((float)((rand() % 90) + j * 90 ));
//            angle = 0;
            glm::vec3 rotatedLocalTrans = glm::rotate(localTrans, angle, glm::vec3(0, 1, 0));
            
            glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(x, landscape->positionAt(rotatedLocalTrans.x + x, rotatedLocalTrans.z + z).y, z));
            trans = glm::rotate(trans, angle, glm::vec3(0, 1, 0));
            trans = glm::translate(trans, localTrans);
            glm::mat4 finalTrans = glm::translate(glm::mat4(), vec3(trans[3]));
            trees->push_back(finalTrans);
        }
    }
    
    int numFlowers = 75;
    for (int i = 0; i < numFlowers; i++) {
        float x = ((rand() % terrainWidth) - terrainWidth/2) * scale * 0.75;
        float z = ((rand() % terrainHeight) - terrainHeight/2) * scale * 0.75;
        grassPositions->push_back(glm::vec3(landscape->positionAt(x, z)));
        grassSizes->push_back(glm::vec3(0.05));
        flowersWatered.push_back(false);
    }
    
    maxCut = trees->size();
    
    forest = new Forest(treeShader, trees);

    
    vector<const GLchar*> faces;
    faces.push_back("Resources/Skyboxes/Sky/right.jpg");
    faces.push_back("Resources/Skyboxes/Sky/left.jpg");
    faces.push_back("Resources/Skyboxes/Sky/top.jpg");
    faces.push_back("Resources/Skyboxes/Sky/bottom.jpg");
    faces.push_back("Resources/Skyboxes/Sky/back.jpg");
    faces.push_back("Resources/Skyboxes/Sky/front.jpg");
    
    skybox = new Skybox(faces, manager.manageShader(SHADER_SKY, "ModelShaders/SimpleSky"));
    
    characters = new vector<Character * >();
    
    fallers = new vector<Character * >();
    
    standers = new vector<Character * >();

    int numJumperPokemon = 8;
    string jumperPokemon[8] = {
        "Pikachu", "Eevee", "Munchlax", "Gible", "Meowth", "Mankey", "Arcanine",
        "Aipom"
    };
    
    jumpers = new vector<Character * >();
    int numJumpers = 40;
    float jscale = scale * 0.75;
    for (int i = 0; i < numJumpers; i++) {
        double r1 = (double)rand()/RAND_MAX * 2 - 1;
        double r2 = (double)rand()/RAND_MAX * 2 - 1;
        glm::vec3 pos = glm::vec3(r1 * terrainWidth/2 * jscale, 0, r2 * terrainHeight/2 * jscale);
        int pokemon = (int)((double)rand()/RAND_MAX * numJumperPokemon);
        Character * c = new Character("Resources/Models/" + jumperPokemon[pokemon] + "/" + jumperPokemon[pokemon] + ".dae", textureModelShader, new JumpAnimation());
        c->translate(landscape->positionAt(pos.x, pos.z));
        c->strafe((double)rand()/RAND_MAX * 360);
        jumpers->push_back(c);
        characters->push_back(c);
    }
    
    flyers = new vector<Character * >();
    rotations = new vector<glm::vec3>();
    int numFlyers = 20;
    float fscale = scale * 0.75;
    for (int i = 0; i < numFlyers; i++) {
        double r1 = (double)rand()/RAND_MAX * 2 - 1;
        double r2 = (double)rand()/RAND_MAX * 2 - 1;
        glm::vec3 rPos = glm::vec3(r1 * terrainWidth/2 * fscale, 0, r2 * terrainHeight/2 * fscale);
        rotations->push_back(rPos);
        string path;
        if (i < 4) path = "Resources/Models/Pidgeotto/Pidgeotto.dae";
        else if (i < 8) path = "Resources/Models/Staravia/Staravia.dae";
        else if (i < 12) path = "Resources/Models/Salamence/Salamence.dae";
        else if (i < 16) path = "Resources/Models/Scyther/Scyther.dae";
        else path = "Resources/Models/Gliscor/Gliscor.dae";
        Character * f = new Character(path, textureModelShader, new FlyAnimation(90 * (double)rand()/RAND_MAX, rPos));
        double c = 20 * (double)rand()/RAND_MAX;
        f->translate(landscape->positionAt(rPos.x, rPos.z) + glm::vec3(c, 25 * (double)rand()/RAND_MAX + 50, c));
        if (rPos.x > 0)  {
            if (rPos.z < 0) {
                f->strafe(270.0f);
            } else  {
                f->strafe(180.0f);
            }
        }
        if (rPos.x < 0)  {
            if (rPos.z > 0) {
                 f->strafe(90.0f);
            }
        }
        f->rot(30.0f, glm::vec3(0,1, 0));
        flyers->push_back(f);
        characters->push_back(f);
    }
    
    ui = new UI(manager.manageShader(SHADER_UI, "UI"), width, height, manager.manageShader(SHADER_FONT, "font"));
    
    manager.manageShader(SHADER_REFLECTION, "reflect");
    
    FMOD_RESULT result;
    unsigned int version;
    int numDrivers;
    FMOD_SPEAKERMODE speakerMode;
//    FMOD_CAPS caps;
    char name[256];
    
    // Create FMOD interface object
    result = FMOD::System_Create(&system);
    FMODErrorCheck(result);
    
    // Check version
    result = system->getVersion(&version);
    FMODErrorCheck(result);
    
    if (version < FMOD_VERSION) {
        std::cout << "Error! You are using an old version of FMOD " << version << ". This program requires " << FMOD_VERSION << std::endl;
    }
    
    // Get number of sound cards result = system->getNumDrivers(&numDrivers);
    FMODErrorCheck(result);
    
    // No sound cards (disable sound)
    if (numDrivers == 0) {
        result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        FMODErrorCheck(result);
    }
    
    result = system->init(100, FMOD_INIT_NORMAL, 0);
    FMODErrorCheck(result);
    
    // Open music as a stream
    FMOD::Sound *song1;
    result = system->createStream("Resources/Sounds/vaniville-town.mp3", FMOD_DEFAULT, 0, &song1);
    FMODErrorCheck(result);
    
    // Assign each song to a channel and start them paused
    FMOD::Channel *channel1 = 0;
    
    song1->setMode(FMOD_LOOP_NORMAL);

    result = system->playSound(song1, 0, false, &channel1);
    FMODErrorCheck(result);
    result = channel1->setVolume(0.5f);
    FMODErrorCheck(result);
    
    system->createStream("Resources/Sounds/Collision.wav", FMOD_DEFAULT, 0, &collisionSound);
    
    system->createStream("Resources/Sounds/Cut.wav", FMOD_DEFAULT, 0, &cutSound);
    
    system->createStream("Resources/Sounds/Water.wav", FMOD_DEFAULT, 0, &waterSound);
    
    system->createStream("Resources/Sounds/Fall.wav", FMOD_DEFAULT, 0, &fallSound);
    
    lastTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

void World::moveCharacter(float direction, float deltaTime) {
    float height = landscape->positionAt(player->position().x, player->position().z).y;
    glm::vec3 trans = player->forward(direction, deltaTime);
    float delta = landscape->positionAt(player->position().x, player->position().z).y - height;
    float threshold = 0.5;
    if (delta > threshold) {
        extra += (delta/2);
        delta = delta/2;
    }
    if (extra > 0) {
        delta += fmin(extra, threshold);
        extra -= fmin(extra, threshold);
    }
    player->addHeight(delta);
    trans.y += delta;
    camera->move(trans);
}

void World::update() {
    
    system->update();
    
    unsigned long deltaTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1) - lastTime;
    lastTime = lastTime + deltaTime;
    
    for (int i = 0; i < jumpers->size(); i++) {
        float distance = 1.0/4.0;
        Character * jumper = jumpers->at(i);
        
        glm::vec3 oldPos = jumper->position();
        
        float y = landscape->positionAt(jumper->position().x, jumper->position().z).y;
        jumper->forward(distance, deltaTime/200.0);
        
        if (landscape->outofBounds(jumper->position().x, jumper->position().z)) {
            float r = ((double)rand()/RAND_MAX * 2 - 1) * 45;
            jumper->strafe(180 + r);
            jumper->translate(oldPos - jumper->position());
        } else {
            float delta = landscape->positionAt(jumper->position().x, jumper->position().z).y - y;
            jumper->addHeight(delta);
        }
        
    }
    
    for (int i = 0; i < fallers->size(); i++) {
        Character * faller = fallers->at(i);
        faller->forward(0, deltaTime);
    }
    
    for (int i = 0; i < flyers->size(); i++) {
        Character * flyer = flyers->at(i);
        flyer->forward(0, deltaTime);
    }
    
    int * lastKey = keyboard->queryInput();
    if (lastKey != NULL) {
        
        GLuint modelShader = manager.retrieveShader(SHADER_TEXTURED_MODEL);
        if (*lastKey == GLFW_KEY_1 && curPlayer != 0) {
            curPlayer = 0;
            glm::vec3 pos = player->position();
            player = new Player("Resources/Models/Bulbasaur/Bulbasaur.dae", modelShader,new RotationAnimation(500.0f));
            player->translate(pos);
        } else if (*lastKey == GLFW_KEY_2 && curPlayer != 1) {
            curPlayer = 1;
            glm::vec3 pos = player->position();
            player = new Player("Resources/Models/Squirtle/Squirtle.dae", modelShader,new RotationAnimation(500.0f));
            player->translate(pos);
        } else if (*lastKey == GLFW_KEY_3 && curPlayer != 2) {
            curPlayer = 2;
            glm::vec3 pos = player->position();
            player = new Player("Resources/Models/Charmander/Charmander.dae", modelShader,new RotationAnimation(500.0f));
            player->translate(pos);
        }
        
        glm::vec3 b2 = player->getMin();
        glm::vec3 b1 = player->getMax();
        
        if (*lastKey == GLFW_KEY_SPACE) {
            modelMode = !modelMode;
            keyboard->remove(GLFW_KEY_SPACE);
        }
        
        if (modelMode) {
            ui->interactModel(*lastKey, deltaTime/10.0f);
        } else {
            if (*lastKey == GLFW_KEY_D) {
                player->strafe(-2);
            } else if (*lastKey == GLFW_KEY_A) {
                player->strafe(2);
            } else if (*lastKey == GLFW_KEY_W) {
                moveCharacter(0.5, deltaTime);
                if (forest->intersect(b1, b2, player->modelMatrix())) {
                    moveCharacter(-0.5, deltaTime);
                    FMOD::Sound * current;
                    channel3->getCurrentSound(&current);
                    if (current != collisionSound) {
                        system->playSound(collisionSound, 0, false, &channel3);
                        channel3->setVolume(1.0f);
                    }
                }
            } else if (*lastKey == GLFW_KEY_S) {
                moveCharacter(-0.5, deltaTime);
                if (forest->intersect(b1, b2, player->modelMatrix())) {
                    moveCharacter(0.5, deltaTime);
                    FMOD::Sound * current;
                    channel3->getCurrentSound(&current);
                    if (current != collisionSound) {
                        system->playSound(collisionSound, 0, false, &channel3);
                        channel3->setVolume(1.0f);
                    }
                }
            }
        }
        
        if (*lastKey == GLFW_KEY_B) {
            reflect = !reflect;
            keyboard->remove(GLFW_KEY_B);
        }
        
        if (*lastKey == GLFW_KEY_X) {
            box = !box;
            keyboard->remove(GLFW_KEY_X);
        }
        
        if (*lastKey == GLFW_KEY_L) {
            wire = !wire;
            keyboard->remove(GLFW_KEY_L);
        }
        
        if (*lastKey == GLFW_KEY_ENTER) {
            glm::vec3 view;
            glm::vec3 pos;
            if (firstPerson) {
                view = camera->getView();
                pos = camera->getPosition();
            } else {
                view = player->viewDirection();
                pos = player->position();
            }
            glm::vec3 treePos;
            int sIdx;
            if(forest->shake(view, player->position(), treePos, sIdx) && (double)rand()/RAND_MAX < 0.50f) {
                if (length(treePos - player->position()) <= 15) {
                    if (curPlayer == 2) {
                        float height = 5;
                        glm::vec3 d = normalize(player->position() - treePos);
                        d.y = 5;
                        treePos.y += height;
                        Character * f = new Character("Resources/Models/Kakuna/Kakuna.dae", manager.retrieveShader(SHADER_TEXTURED_MODEL), new FallAnimation(d, 5, height));
                        f->translate(treePos);
                        fallers->push_back(f);
                        characters->push_back(f);
                        keyboard->remove(GLFW_KEY_ENTER);
                        system->playSound(fallSound, 0, false, &channel3);
                        channel3->setVolume(1.0f);
                    } else if (curPlayer == 0) {
                        forest->cut(sIdx);
                        cut++;
                        system->playSound(cutSound, 0, false, &channel3);
                        channel3->setVolume(1.0f);
                    }
                }
            } else if (curPlayer == 1) {
                if (view.y == 0) view.y = -0.1;
                int i = 0;
                float minIdx = -1;
                float min = FLT_MAX;
                for(vector<glm::vec3>::iterator it = grassPositions->begin(); it != grassPositions->end(); ++it) {
                    float t;
                    glm::vec3 tb2 = *it + glm::vec3(1, 5, 1) * grassSizes->at(i) * grassA->getMin();
                    glm::vec3 tb1 = *it + glm::vec3(1, 5, 1) * grassSizes->at(i) * grassA->getMax();
                    if (rayBox(tb2, tb1, pos, view, t)) {
                        if (t < min && t > 0 && t < 15) {
                            min = t;
                            minIdx = i;
                        }
                    }
                    i++;
                }
                if (minIdx != -1 && min >= 0 && min <= 15) {
                    if (grassSizes->at(minIdx).x <= 1.5) {
                        grassSizes->at(minIdx) *= 1.01;
                        if (!flowersWatered.at(minIdx)) {
                            flowersWatered.at(minIdx) = true;
                            watered++;
                        }
                        FMOD::Sound * current;
                        channel3->getCurrentSound(&current);
                        if (current != waterSound) {
                            system->playSound(waterSound, 0, false, &channel3);
                            channel3->setVolume(1.0f);
                        }
                    }
                }
            }
        }
        
        if (!firstPerson) {
        
            if (*lastKey == GLFW_KEY_UP) {
                camera->roll((float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_DOWN) {
                camera->roll(-(float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_LEFT) {
                camera->pan(-(float)(deltaTime * 90.0f/1000.0f));
            } else if (*lastKey == GLFW_KEY_RIGHT) {
                camera->pan((float)(deltaTime * 90.0f/1000.0f));
            }
        
        } else {
            if (*lastKey == GLFW_KEY_UP) {
                camera->spin((float)(deltaTime * 90.0f/1000.0f));
                if (dot(camera->getView(), player->viewDirection()) < 0) {
                    camera->spin(-(float)(deltaTime * 90.0f/1000.0f));
                }
            } else if (*lastKey == GLFW_KEY_DOWN) {
                camera->spin(-(float)(deltaTime * 90.0f/1000.0f));
                if (dot(camera->getView(), player->viewDirection()) < 0) {
                    camera->spin((float)(deltaTime * 90.0f/1000.0f));
                }
            } else if (*lastKey == GLFW_KEY_LEFT) {
                camera->rotate(-(float)(deltaTime * 90.0f/1000.0f));
                if (dot(camera->getView(), player->viewDirection()) < 0) {
                    camera->rotate((float)(deltaTime * 90.0f/1000.0f));
                }
            } else if (*lastKey == GLFW_KEY_RIGHT) {
                camera->rotate((float)(deltaTime * 90.0f/1000.0f));
                if (dot(camera->getView(), player->viewDirection()) < 0) {
                    camera->rotate(-(float)(deltaTime * 90.0f/1000.0f));
                }
            }
        }
    
        if (*lastKey == GLFW_KEY_V) {
            if (!firstPerson) {
                glm::vec3 max = (player->modelMatrix() * glm::vec4(player->getMax(), 1)).xyz();
                glm::vec3 min = (player->modelMatrix() * glm::vec4(player->getMin(), 1)).xyz();
                glm::vec3 pos = glm::vec3((max.x + min.x)/2, max.y, max.z);
                camera->look(player->viewDirection() + pos, pos);
            } else {
                camera->look(player->position());
            }
            firstPerson = !firstPerson;
            keyboard->remove(GLFW_KEY_V);
            lastPath = player->path;
        }
        
    }
}

void World::render() {
    glViewport(0, 0, width, height);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDepthFunc(GL_LEQUAL);
    if (box) {
        bindShader(SHADER_SKY);
        skybox->render();
    }
    if (reflect && box) {
        GLuint reflectShader = bindShader(SHADER_REFLECTION);
        glm::vec3 viewPos = camera->getPosition();
        glUniform3f(glGetUniformLocation(reflectShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
        player->renderReflection();
    }
    glDepthFunc(GL_LESS);
    
    if (wire) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
  
    glm::vec3 viewPos = camera->getPosition();
    glm::vec3 lightPos = glm::vec3(-100, 600, 100);
    
    GLuint modelShader = bindShader(SHADER_TEXTURED_MODEL);
    glUniform3f(glGetUniformLocation(modelShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(modelShader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    
    
    player->render();
    for (int i = 0; i < jumpers->size(); i++) {
        jumpers->at(i)->render();
    }
    for (int i = 0; i < flyers->size(); i++) {
        flyers->at(i)->render();
    }
    for (int i = 0; i < fallers->size(); i++) {
        fallers->at(i)->render();
    }
    for (int i = 0; i < standers->size(); i++) {
        standers->at(i)->render();
    }
    for (int i = 0; i < grassPositions->size(); i++) {
        grassA->resetTranslation();
        grassA->translate(grassPositions->at(i));
        grassA->resetTransform();
        grassA->scale(grassSizes->at(i));
        grassA->render();
    }
    
    
    bindShader(SHADER_GROUND);
    landscape->render();

    GLuint treeShader = bindShader(SHADER_TREE);
    glUniform3f(glGetUniformLocation(treeShader, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(treeShader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    forest->render();
    
    string path = "";
    GLuint shader;
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    if (firstPerson) {
        
        float t = FLT_MAX;
        float min = FLT_MAX;
        bool hit = false;
        float idx = -1;
        for (int i = 0; i < characters->size(); i++) {
            Character * c = characters->at(i);
            glm::vec3 b1 = (c->modelMatrix() * glm::vec4(c->getMin(), 1)).xyz();
            glm::vec3 b2 = (c->modelMatrix() * glm::vec4(c->getMax(), 1)).xyz();
            
            if (b1.x > b2.x) {
                float temp = b2.x;
                b2.x = b1.x;
                b1.x = temp;
            }
            
            if (b1.y > b2.y) {
                float temp = b2.y;
                b2.y = b1.y;
                b1.y = temp;
            }
            
            if (b1.z > b2.z) {
                float temp = b2.z;
                b2.z = b1.z;
                b1.z = temp;
            }
            
            if (rayBox(b1, b2, camera->getPosition(), camera->getView(), t)) {
                if (t < min && t > 0) {
                    min = t;
                    idx = i;
                    hit = true;
                    path = characters->at(idx)->path;
                    shader = modelShader;
                    ui->setTags("This is a Pokemon.", "Find them all!");
                }
            }
        }
        if(hit && idx != -1 && std::find(pokemonSeen.begin(), pokemonSeen.end(), path) == pokemonSeen.end()) {
            seen++;
            pokemonSeen.push_back(path);
        }
        
        if (forest->intersect(camera->getPosition(), camera->getView(), t)) {
            if (t < min && t > 0) {
                min = t;
                hit = true;
                ui->setTags("This is a Tree.", "Cut them all!");
            }
        }
        
        int i = 0;
        for(vector<glm::vec3>::iterator it = grassPositions->begin(); it != grassPositions->end(); ++it) {
            float t;
            glm::vec3 tb2 = *it + grassSizes->at(i) * grassA->getMin();
            glm::vec3 tb1 = *it + grassSizes->at(i) * grassA->getMax();
            if (rayBox(tb2, tb1, camera->getPosition(), camera->getView(), t)) {
                if (t < min && t > 0) {
                    min = t;
                    hit = true;
                    ui->setTags("This is a Flower.", "Water them all!");
                }
            }
            i++;
        }
        
        
        if (!hit) {
            ui->setTags(NULL, NULL);
        }
        
        bindShader(SHADER_UI);
        ui->renderCrossHair(hit);
    } else {
        path = player->path;
        shader = modelShader;
        ui->setTags(NULL, NULL);
    }
    
    bindShader(SHADER_UI);
    ui->render(path, shader);
    if (path != "") {
        char * name = (char *)path.substr(path.find_last_of("/") + 1, path.find_last_of(".") - path.find_last_of("/") - 1).c_str();
        for(int i = 0; i < strlen(name); ++i)
            name[i] = toupper(name[i]);
        ui->setName(name);
        
        if (lastPath != path) {
            if(cries[name] == NULL) {

                // Open music as a stream
                FMOD::Sound *cry;
                stringstream ss;
                ss << "Resources/Sounds/" << name << ".wav";
                system->createStream(ss.str().c_str(), FMOD_DEFAULT, 0, &cry);
                
                system->playSound(cry, 0, false, &channel2);
                channel2->setVolume(1.0f);
                cries[name] = cry;
            } else {
                system->playSound(cries[name], 0, false, &channel2);
            }
            lastPath = path;
        }
        
    }
    
    ui->setSeen(seen, 15);
    ui->setWatered(watered, flowersWatered.size());
    ui->setCuts(cut, maxCut);
    
    glDisable( GL_BLEND );
}

GLuint World::bindShader(int shaderKey) {
    GLuint shader = manager.retrieveShader(shaderKey);
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, value_ptr(camera->getViewMatrix()));
    return shader;
}