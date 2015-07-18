//
//  Intersection.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-18.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "Intersection.h"

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
    if ( (tmin > tymax) || (tymin > tmax) )
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
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
    return t > 0;
//    if (tzmin > tmin)
//        tmin = tzmin;
//    if (tzmax < tmax)
//        tmax = tzmax;
//    return ( (tmin < t1) && (tmax > t0) );
}