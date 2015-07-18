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

bool pointInBox(glm::vec3 b1, glm::vec3 b2, float x, float y, float z) {
    int xmin = min(b1.x, b2.x);
    int xmax = max(b1.x, b2.x);
    
    int ymin = min(b1.y, b2.y);
    int ymax = max(b1.y, b2.y);
    
    int zmin = min(b1.z, b2.z);
    int zmax = max(b1.z, b2.z);
    
    if (x < xmin || x > xmax) return false;
    if (y < ymin || y > ymax) return false;
    if (z < zmin || z > zmax) return false;
    
    return true;
}

#endif
