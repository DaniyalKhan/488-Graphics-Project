//
//  Forest.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-13.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Forest.h"

Forest::Forest(GLuint s, vector<glm::mat4> * translations) : shader(s) {
    tree = new Model("ModelFiles/LowPolyTree/tree.3ds");
    tree->setShader(shader);
    positions = translations;
    tree->scale(glm::vec3(4, 4, 4));
    tree->applyColor(glm::vec3(0.22, 0.68f, 0.28f), 1);
    tree->applyColor(glm::vec3(0.68f, 0.51f, 0.22f), 0);
}

void Forest::render() {
    for(vector<glm::mat4>::iterator it = positions->begin(); it != positions->end(); ++it) {
        glUniformMatrix4fv(glGetUniformLocation(shader, "translation"), 1, GL_FALSE, glm::value_ptr(*it));
        tree->render();
    }
    
}