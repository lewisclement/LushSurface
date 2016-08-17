#pragma once
#ifndef GENERATOR_H
#define GENERATOR_H

#include "../pch.hpp"
#include "simplexnoise.hpp"
#include "simplex.hpp"


namespace Generate {
    enum Generator : int {originalSimplex = 0, FBMWarpedFlow};

    static float generateValue(Generator generator, unsigned int seed, const float low, const float high, const glm::vec2 coord);
    static float generateValue(Generator generator, unsigned int seed, const float low, const float high, const float x, const float y);

    static float generateValue(Generator generator, unsigned int seed, const float low, const float high, const glm::vec2 coord) {
        if(generator == originalSimplex) {
            SimplexNoise* noise = new SimplexNoise();
            noise->setSeed(seed);
            return noise->scaled_raw_noise_2d(low, high, coord.x, coord.y);
        } else if (generator == FBMWarpedFlow) {
            float r = (Simplex::flowNoise( coord + Simplex::fBm( coord ), 0.0f ))   * (high - low) / 2 + (high + low) / 2;
            std::cout << low << "<" << r << "<" << high << std::endl;
            return r;
        }

        return 0;
    }

    static float generateValue(Generator generator, unsigned int seed, const float low, const float high, const float x, const float y) {
        return generateValue(generator, seed, low, high, glm::vec2(x, y));
    }
}

#endif
