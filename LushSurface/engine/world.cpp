#include "world.hpp"

//temporary
float scaled_raw_noise_2d(const float loBound, const float hiBound, const float x, const float y);
//temporary

Coordinate::Coordinate(int32_t X, int32_t Y) {
    x = X;
    y = Y;
}

TerrainColumn::TerrainColumn(uint16_t Bottom, uint16_t Top) {
    bottom = Bottom;
    top = Top;
}

World::World(float x, float y) {
    // Build the broadphase
    broadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // The actual physics solver
    solver = new btSequentialImpulseConstraintSolver;

    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -30, 0));



    int32_t centerChunkX = int32_t(x / chunkSize);
    int32_t centerChunkY = int32_t(y / chunkSize);

    generator = new SimplexNoise();
    generator->setSeed(34142345);

    for(int i = 0; i < loadedWorldSize * loadedWorldSize; i++) {
        Chunk *chunk = new Chunk(generator);
        chunk->initialize(centerChunkX + i % 3 - 1, centerChunkY + int(i / 3) - 1, dynamicsWorld);
        chunks.push_back(chunk);
    }
}

World::~World() {
    for(unsigned long i = 0; i < chunks.size(); i++) {
        delete chunks[i];
    }

    delete generator;

    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void World::addEntity(Entity* entity) {
    entities.push_back(entity);
    dynamicsWorld->addRigidBody(entity->getRigidBody());
}

void World::processPhysics(GLuint deltaTime) {
    dynamicsWorld->stepSimulation(deltaTime, 1);

    for(int i = 0; i < entities.size(); i++) {
        entities[i]->actualize();
    }
}

bool World::loadTerrain(float x, float y) {
    if(x < 0) x -= chunkSize;
    if(y < 0) y -= chunkSize;

    if(!(int(x / chunkSize) == focusChunkX && int(y / chunkSize) == focusChunkY)) {
        int32_t newFocusChunkX = int32_t(x / chunkSize);
        int32_t newFocusChunkY = int32_t(y / chunkSize);

        int xDir = newFocusChunkX - focusChunkX;
        int yDir = newFocusChunkY - focusChunkY;

        if(xDir == 0 && yDir != 0) {
            Chunk *chunk = getChunk(focusChunkX, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX, newFocusChunkY + yDir, dynamicsWorld);

            chunk = getChunk(focusChunkX - 1, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX - 1, newFocusChunkY + yDir, dynamicsWorld);

            chunk = getChunk(focusChunkX + 1, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX + 1, newFocusChunkY + yDir, dynamicsWorld);
        } else if(xDir != 0 && yDir == 0) {
            Chunk *chunk = getChunk(focusChunkX - xDir, focusChunkY);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY, dynamicsWorld);

            chunk = getChunk(focusChunkX - xDir, focusChunkY - 1);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY - 1, dynamicsWorld);

            chunk = getChunk(focusChunkX - xDir, focusChunkY + 1);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY + 1, dynamicsWorld);
        } else {
            Chunk *chunk = getChunk(focusChunkX - xDir, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY + yDir, dynamicsWorld);

            chunk = getChunk(focusChunkX, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX, newFocusChunkY + yDir, dynamicsWorld);

            chunk = getChunk(focusChunkX - xDir, focusChunkY);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY, dynamicsWorld);

            if(xDir == yDir) {
                chunk = getChunk(focusChunkX - xDir, focusChunkY + yDir);
                chunk->initialize(newFocusChunkX - xDir, newFocusChunkY + yDir, dynamicsWorld);

                chunk = getChunk(focusChunkX + xDir, focusChunkY - yDir);
                chunk->initialize(newFocusChunkX + xDir, newFocusChunkY - yDir, dynamicsWorld);
            } else {
                chunk = getChunk(focusChunkX - xDir, focusChunkY - yDir);
                chunk->initialize(newFocusChunkX - xDir, newFocusChunkY - yDir, dynamicsWorld);

                chunk = getChunk(focusChunkX + xDir, focusChunkY + yDir);
                chunk->initialize(newFocusChunkX + xDir, newFocusChunkY + yDir, dynamicsWorld);
            }
        }

        focusChunkX = newFocusChunkX;
        focusChunkY = newFocusChunkY;

        return true;
    }

    return false;
}

Chunk* World::getChunk(int32_t x, int32_t y) {
    Chunk *returnChunk = NULL;
    for(unsigned long i = 0; i < chunks.size(); i++) {
        Coordinate pos = chunks[i]->getPosition();
        if(pos.x == x && pos.y == y) {returnChunk = chunks[i]; break;}
    }
    return returnChunk;
}

std::vector<Coordinate> World::getChunkCoordinates() {
    std::vector<Coordinate> coords;

    for(unsigned long i = 0; i < chunks.size(); i++) {
        coords.push_back(chunks[i]->getPosition());
    }

    return coords;
}

TerrainColumn World::getTerrain(int32_t x, int32_t y) {
    int32_t chunkX = int32_t(x / chunkSize);
    int32_t chunkY = int32_t(y / chunkSize);

    uint16_t relativeX = x % chunkSize;
    uint16_t relativeY = y % chunkSize;

    if(x < 0) {
        chunkX--;
        relativeX = chunkSize + relativeX;
    }
    if(y < 0) {
        chunkY--;
        relativeY = chunkSize + relativeY;
    }

    return getChunk(chunkX, chunkY)->getTerrain(relativeX, relativeY);
}
