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
    int32_t centerChunkX = int32_t(x / chunkSize);
    int32_t centerChunkY = int32_t(y / chunkSize);

    generator = new SimplexNoise();
    generator->setSeed(123456789);

    for(int i = 0; i < loadedWorldSize * loadedWorldSize; i++) {
        Chunk *chunk = new Chunk(generator);
        chunk->initialize(centerChunkX + i % 3 - 1, centerChunkY + int(i / 3) - 1);
        chunks.push_back(chunk);
    }
}

World::~World() {
    for(unsigned long i = 0; i < chunks.size(); i++) {
        delete chunks[i];
    }
}

bool World::loadTerrain(float x, float y) {
    if(!(int(x / chunkSize) == focusChunkX && int(y / chunkSize) == focusChunkY)) {
        int32_t newFocusChunkX = int32_t(x / chunkSize);
        int32_t newFocusChunkY = int32_t(y / chunkSize);

        int xDir = newFocusChunkX - focusChunkX;
        int yDir = newFocusChunkY - focusChunkY;

        if(xDir == 0 && yDir != 0) {
            Chunk *chunk = getChunk(focusChunkX, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX, newFocusChunkY + yDir);

            chunk = getChunk(focusChunkX - 1, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX - 1, newFocusChunkY + yDir);

            chunk = getChunk(focusChunkX + 1, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX + 1, newFocusChunkY + yDir);
        } else if(xDir != 0 && yDir == 0) {
            Chunk *chunk = getChunk(focusChunkX - xDir, focusChunkY);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY);

            chunk = getChunk(focusChunkX - xDir, focusChunkY - 1);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY - 1);

            chunk = getChunk(focusChunkX - xDir, focusChunkY + 1);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY + 1);
        } else {
            Chunk *chunk = getChunk(focusChunkX - xDir, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY + yDir);

            chunk = getChunk(focusChunkX, focusChunkY - yDir);
            chunk->initialize(newFocusChunkX, newFocusChunkY + yDir);

            chunk = getChunk(focusChunkX - xDir, focusChunkY);
            chunk->initialize(newFocusChunkX + xDir, newFocusChunkY);

            if(xDir == yDir) {
                chunk = getChunk(focusChunkX - xDir, focusChunkY + yDir);
                chunk->initialize(newFocusChunkX - xDir, newFocusChunkY + yDir);

                chunk = getChunk(focusChunkX + xDir, focusChunkY - yDir);
                chunk->initialize(newFocusChunkX + xDir, newFocusChunkY - yDir);
            } else {
                chunk = getChunk(focusChunkX - xDir, focusChunkY - yDir);
                chunk->initialize(newFocusChunkX - xDir, newFocusChunkY - yDir);

                chunk = getChunk(focusChunkX + xDir, focusChunkY + yDir);
                chunk->initialize(newFocusChunkX + xDir, newFocusChunkY + yDir);
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
