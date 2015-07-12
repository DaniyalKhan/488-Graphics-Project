//
//  PerlinNoise.h
//  Project
//
//  Created by Daniyal Khan on 2015-07-10.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#ifndef __Project__PerlinNoise__
#define __Project__PerlinNoise__

#include <stdio.h>
#include <cstdlib>

class PerlinNoise {
private:
    float ** noise;
    int width, height;
    float interpolate(float x0, float x1, float alpha);
    float ** smoothNoise(int octave);
public:
    PerlinNoise(int width, int height);
    float ** perlinNoise(int octaveCount);
};

#endif /* defined(__Project__PerlinNoise__) */
