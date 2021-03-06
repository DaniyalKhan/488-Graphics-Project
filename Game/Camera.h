//
//  Camera.h
//  Project
//
//  Created by Daniyal Khan on 2015-06-28.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__Camera__
#define __Project__Camera__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;
public:
    Camera(glm::vec3 lookAt);
    glm::mat4 getViewMatrix();
    void pan(float degrees);
    void roll(float degrees);
    void look(glm::vec3 lookAt);
    void look(glm::vec3 lookAt, glm::vec3 position);
    void move(glm::vec3 trans);
    void rotate(float degrees);
    void spin(float degrees);
    
    glm::vec3 getPosition() {
        return position;
    }
    
    glm::vec3 getView() {
        return lookAt - position;
    }
    
};

#endif /* defined(__Project__Camera__) */
