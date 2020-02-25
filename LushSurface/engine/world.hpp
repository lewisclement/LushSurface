#ifndef WORLD_H
#define WORLD_H

#include "../pch.hpp"
#include "generator.hpp"
#include "entity.hpp"

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
    Chunk(unsigned int seed);
    ~Chunk();

    void initialize(int32_t x, int32_t y, btDiscreteDynamicsWorld *dynamicsWorld);

    void bindVBO();

    void addCube(uint16_t x, uint16_t y, uint16_t z);

    Coordinate getPosition();
    TerrainColumn getTerrain(uint16_t x, uint16_t y);
    GLint getTriangleCount();
    bool isLoaded();

private:
    void generateChunkv1();
    void generateChunkv2();
    void generateChunkv3();
    void generateChunkv4();
    void generateChunkv5();
    void generateChunkv6();

    void fillVertexes();

    int32_t xPos, yPos;
    std::vector<std::vector<uint16_t> > worldHeightDataLower;
    std::vector<std::vector<uint16_t> > worldHeightDataUpper;

    std::vector<GLfloat> vertices;
    std::vector<uint16_t> blockHeights;

    btTriangleMesh* mesh;
    btCollisionShape* collisionShape;
    btRigidBody* rigidBody;

    GLuint VAO = 0, VBO = 0;
    GLint pointCount = 0;
    bool loaded = false;

    unsigned int seed;
};

class World{
public:
    World(float x, float y);
    ~World();

    void addEntity(Entity* entity);
    void processPhysics(GLuint deltaTime);

    bool loadTerrain(float x, float y);

    int32_t focusChunkX = 0.0f, focusChunkY = 0.0f;

    Chunk* getChunk(int32_t x, int32_t y);
    std::vector<Coordinate> getChunkCoordinates();
    TerrainColumn getTerrain(int32_t x, int32_t y);
    std::vector<Chunk*> getChunks();

    btDiscreteDynamicsWorld* dynamicsWorld;

private:
    std::vector<Chunk*> chunks;

    std::vector<Entity*> entities;

    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
};

#endif // WORLD_H
