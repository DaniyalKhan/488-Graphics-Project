//
//  Intersection.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-14.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef Project_Intersection_h
#define Project_Intersection_h

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/ext.hpp"
#include <math.h>

bool rayBox(glm::vec3 b1, glm::vec3 b2, glm::vec3 pos, glm:: vec3 dir, float &t);
bool pointInBox(glm::vec3 b1, glm::vec3 b2, float x, float y, float z, glm::mat4 transform);
#endif
