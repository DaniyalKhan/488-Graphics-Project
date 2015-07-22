//
//  Intersection.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-18.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Intersection.h"
#include <iostream>

using namespace std;

bool pointInBox(glm::vec3 b1, glm::vec3 b2, float x, float y, float z, glm::mat4 transform) {
    
    glm::vec3 v = (transform * glm::vec4(x, y, z, 1)).xyz();
    x = v.x; y = v.y; z = v.z;
    
    float xmin = fmin(b1.x, b2.x);
    float xmax = fmax(b1.x, b2.x);
    
    float ymin = fmin(b1.y, b2.y);
    float ymax = fmax(b1.y, b2.y);
    
    float zmin = fmin(b1.z, b2.z);
    float zmax = fmax(b1.z, b2.z);
    
    if (x < xmin || x > xmax) return false;
    if (y < ymin || y > ymax) return false;
    if (z < zmin || z > zmax) return false;
    
    return true;
}

bool rayBox(glm::vec3 b1, glm::vec3 b2, glm::vec3 pos, glm:: vec3 dir, float &t) {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    if (dir.x >= 0) {
        tmin = (b1.x - pos.x) / dir.x;
        tmax = (b2.x - pos.x) / dir.x;
    }
    else {
        tmin = (b2.x - pos.x) / dir.x;
        tmax = (b1.x - pos.x) / dir.x;
    }
    if (dir.y >= 0) {
        tymin = (b1.y - pos.y) / dir.y;
        tymax = (b2.y - pos.y) / dir.y;
    }
    else {
        tymin = (b2.y - pos.y) / dir.y;
        tymax = (b1.y - pos.y) / dir.y;
    }
    if ( ((tmin > tymax) || (tymin > tmax)) && dir.y != 0)
        return false;
    if (tymin > tmin && dir.y != 0)
        tmin = tymin;
    if (tymax < tmax && dir.y != 0)
        tmax = tymax;
    if (dir.z >= 0) {
        tzmin = (b1.z - pos.z) / dir.z;
        tzmax = (b2.z - pos.z) / dir.z;
    }
    else {
        tzmin = (b2.z - pos.z) / dir.z;
        tzmax = (b1.z - pos.z) / dir.z;
    }
    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;
    t = fmin(tmin, tmax);
//    if (tmin > 0 || tmax > 0) {
//        cout << t  << endl;
//    }
    return tmin > 0 || tmax > 0;
//    if (tzmin > tmin)
//        tmin = tzmin;
//    if (tzmax < tmax)
//        tmax = tzmax;
//    return ( (tmin < t1) && (tmax > t0) );
}