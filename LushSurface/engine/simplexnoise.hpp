#ifndef SIMPLEXNOISE_H
#define SIMPLEXNOISE_H

class SimplexNoise {
public:
    SimplexNoise();
    ~SimplexNoise();

    void setSeed(unsigned int seed);

    float raw_noise_2d( const float x, const float y );
    float scaled_raw_noise_2d( const float loBound, const float hiBound, const float x, const float y );

private:
    int fastfloor( const float x );

    float dot( const int* g, const float x, const float y );
    float dot( const int* g, const float x, const float y, const float z );
    float dot( const int* g, const float x, const float y, const float z, const float w );

    int grad3[12][3];

    short perm[512];
};

#endif
