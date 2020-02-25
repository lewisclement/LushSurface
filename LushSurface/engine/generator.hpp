#pragma once
#ifndef GENERATOR_H
#define GENERATOR_H

#include "../pch.hpp"
#include "simplexnoise.hpp"
#include "simplex.hpp"


namespace Generate {
    enum Generator : int {originalSimplex = 0, FBMWarpedFlow};
    static unsigned int seed = 0;

    static float generateValue(Generator generator, const float low, const float high, const glm::vec2 coord);
    static float generateValue(Generator generator, const float low, const float high, const float x, const float y);

    static float generateValue(Generator generator, const float low, const float high, const glm::vec2 coord) {
        if(generator == originalSimplex) {
            SimplexNoise* noise = new SimplexNoise();
            noise->setSeed(seed);
            float r = noise->scaled_raw_noise_2d(low, high, coord.x, coord.y);
            delete noise;
            return r;
        } else if (generator == FBMWarpedFlow) {
            float r = ((Simplex::flowNoise( coord + Simplex::fBm( coord ), 0.0f ) * 2.0f) * (high - low) / 2 + (high + low) / 2) - 1.0f;
            return r;
        }

        return 0;
    }

    static float generateValue(Generator generator, const float low, const float high, const float x, const float y) {
        return generateValue(generator, low, high, glm::vec2(x, y));
    }

    static void setSeed(unsigned int seed) {
        Generate::seed = seed;
        Simplex::seed(seed);
    }
}

#endif
