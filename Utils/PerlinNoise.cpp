//
//  PerlinNoise.cpp
//  Project
//
//  Created by Daniyal Khan on 2015-07-10.
//  Copyright (c) 2015 Daniyal Khan. All rights reserved.
//

#include "PerlinNoise.h"
//#include <iostream>

PerlinNoise::PerlinNoise(int w, int h) : width(w), height(h) {
    
    noise = new float * [width];
    for (int i = 0; i < width; i++) {
        noise[i] = new float[height];
    }
    srand(0); //Seed to 0 for testing
    
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            noise[i][j] = ((float)rand()) / RAND_MAX;
        }
    }
}

void PerlinNoise::setProperties(float persistance, float amplitude) {
    this->persistance = persistance;
    this->amplitude = amplitude;
}

float ** PerlinNoise::perlinNoise(int octaveCount) {
    float ** pn = new float * [width];
    for (int i = 0; i < width; i++) {
        pn[i] = new float[height];
    }
    float totalAmplitude = 0.0f;
    float sampleAmplitude = amplitude;
    //blend noise together
    for (int octave = octaveCount - 1; octave >= 0; octave--) {
        sampleAmplitude *= persistance;
        totalAmplitude += amplitude;
        
        float ** sn = smoothNoise(octave);
        
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                pn[i][j] += sn[i][j] * sampleAmplitude;
            }
        }
        for (int i = 0; i < width ; ++i){
            delete [] sn[i];
        }
        delete [] sn;
    }
//    for (int i = 0; i < width; i++) {
//        for (int j = 0; j < height; j++) {
//            pn[i][j] /= totalAmplitude;
//        }
//    }
    return pn;
}

float ** PerlinNoise::smoothNoise(int octave) {
    float ** smoothNoise = new float * [width];
    for (int i = 0; i < width; i++) {
        smoothNoise[i] = new float[height];
    }
    
    int samplePeriod = 1 << octave; // calculates 2 ^ k
    float sampleFrequency = 1.0f / samplePeriod;
    
    for (int i = 0; i < width; i++) {
        //calculate the horizontal sampling indices
        int sample_i0 = (i / samplePeriod) * samplePeriod;
        int sample_i1 = (sample_i0 + samplePeriod) % width; //wrap around
        float horizontal_blend = (i - sample_i0) * sampleFrequency;
        
        for (int j = 0; j < height; j++) {
            //calculate the vertical sampling indices
            int sample_j0 = (j / samplePeriod) * samplePeriod;
            int sample_j1 = (sample_j0 + samplePeriod) % height; //wrap around
            float vertical_blend = (j - sample_j0) * sampleFrequency;
            
            //blend the top two corners
            float top = interpolate(noise[sample_i0][sample_j0],
                                    noise[sample_i1][sample_j0], horizontal_blend);
            
            //blend the bottom two corners
            float bottom = interpolate(noise[sample_i0][sample_j1],
                                       noise[sample_i1][sample_j1], horizontal_blend);
            
            //final blend
            smoothNoise[i][j] = interpolate(top, bottom, vertical_blend);
        }
    }
//    for (int i = 0; i < width; i++) {
//        for (int j = 0; j < height; j++) {
//            std::cout << smoothNoise[i][j] << std::endl;
//        }
//    }
    
    return smoothNoise;
}

float PerlinNoise::interpolate(float x0, float x1, float alpha) {
    return x0 * (1 - alpha) + alpha * x1;
}
