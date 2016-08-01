#ifndef WORLD_H
#define WORLD_H

#include "../pch.hpp"
#include "simplexnoise.hpp"

const uint16_t chunkSize = 64;
const uint8_t loadedWorldSize = 3; //3*3 = 9
const uint8_t loadingAnimationHeight = 10;
const uint16_t loadingAnimationTerrainDuration = 1000;

struct Coordinate {
    int32_t x = 0, y = 0;

    Coordinate(int32_t X, int32_t Y);
};

struct TerrainColumn {
    uint16_t bottom = 0, top = 0;

    TerrainColumn(uint16_t Bottom, uint16_t Top);
};

class Chunk{
public:
    Chunk(SimplexNoise *Generator);
    ~Chunk();

    void initialize(int32_t x, int32_t y);

    void bindVBO();

    void addCube(uint16_t x, uint16_t y, uint16_t z);

    Coordinate getPosition();
    TerrainColumn getTerrain(uint16_t x, uint16_t y);
    GLint getTriangleCount();
    bool isLoaded();

private:
    void generateChunkv1();
    void generateChunkv2();

    void fillVertexes();

    int32_t xPos, yPos;
    std::vector<std::vector<uint16_t> > worldHeightDataLower;
    std::vector<std::vector<uint16_t> > worldHeightDataUpper;

    std::vector<GLfloat> vertices;
    std::vector<uint16_t> blockHeights;

    GLuint VAO = 0, VBO = 0;
    GLint pointCount = 0;
    bool loaded = false;
    SimplexNoise *generator = NULL;
};

class World{
public:
    World(float x, float y);
    ~World();

    bool loadTerrain(float x, float y);

    int32_t focusChunkX = 0.0f, focusChunkY = 0.0f;

    Chunk* getChunk(int32_t x, int32_t y);
    std::vector<Coordinate> getChunkCoordinates();
    TerrainColumn getTerrain(int32_t x, int32_t y);

private:
    std::vector<Chunk*> chunks;
    SimplexNoise *generator = NULL;

    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif // WORLD_H
