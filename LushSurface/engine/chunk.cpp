#include "world.hpp"

Chunk::Chunk(SimplexNoise *noiseGenerator) {
    this->generator = noiseGenerator;
}

Chunk::~Chunk() {
    loaded = false;

    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);

    worldHeightDataLower.clear();
    worldHeightDataUpper.clear();
    vertices.clear();
    blockHeights.clear();
}

void Chunk::bindVBO() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if(!loadingDone) {
        const GLulong currentTime = SDL_GetTicks();
        const GLint deltaTime = (GLint)(currentTime - loadTick);
        const GLint totalTime = (GLint)(currentTime - startLoadTick);

        if(totalTime >= loadingAnimationTerrainDuration) {
            unsigned long i = 1, pos = 0;
            while(i < vertices.size()) {
                vertices[i] = blockHeights[pos];
                vertices[i+6] = blockHeights[pos];
                vertices[i+12] = blockHeights[pos];
                vertices[i+18] = blockHeights[pos];
                vertices[i+24] = blockHeights[pos];
                vertices[i+30] = blockHeights[pos];
                i += 36;

                for(int y = 0; y < 4; y++) {
                    vertices[i] = blockHeights[pos];
                    vertices[i+6] = blockHeights[pos];
                    vertices[i+12] = blockHeights[pos] - 1;
                    vertices[i+18] = blockHeights[pos];
                    vertices[i+24] = blockHeights[pos] - 1;
                    vertices[i+30] = blockHeights[pos] - 1;
                    i += 36;
                }

                pos++;
            }

            loadingDone = true;
        } else {
            unsigned long i = 1;
            while(i < vertices.size()) {
                vertices[i] = GLfloat(vertices[i] + deltaTime * (loadingAnimationHeight / (float)loadingAnimationTerrainDuration));

                i += 6;
            }
        }

        loadTick = currentTime;
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), &vertices[0]);
    }
}

Coordinate Chunk::getPosition() {
    return Coordinate(xPos, yPos);
}

TerrainColumn Chunk::getTerrain(uint16_t x, uint16_t y) {
    if(x >= chunkSize) x = chunkSize - 1;
    if(y >= chunkSize) y = chunkSize - 1;

    TerrainColumn returnTerrain(worldHeightDataLower[x][y], worldHeightDataUpper[x][y]);
    return returnTerrain;
}

void Chunk::initialize(int32_t x, int32_t y) {
    if(loaded == true) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &VAO);

        vertices.clear();
        blockHeights.clear();
        worldHeightDataLower.clear();
        worldHeightDataUpper.clear();

        loaded = false;
        triangleCount = 0;
    }

    xPos = x;
    yPos = y;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    for(uint16_t i = 0; i < chunkSize; i++) {
        std::vector<uint16_t> subUp;
        worldHeightDataUpper.push_back(subUp);
        std::vector<uint16_t> subDown;
        worldHeightDataLower.push_back(subDown);

        for(uint16_t j = 0; j < chunkSize; j++) {
            worldHeightDataUpper[i].push_back(uint16_t(generator->scaled_raw_noise_2d(17, 25, (i + xPos * chunkSize) / 40.0f, (j + yPos * chunkSize) / 40.0f)));
            worldHeightDataUpper[i][j] = (uint16_t)std::max(generator->scaled_raw_noise_2d(17, 23, (i + xPos * chunkSize) / 10.0f, (j + yPos * chunkSize) / 10.0f), float(worldHeightDataUpper[i][j]));
            worldHeightDataUpper[i][j] = (uint16_t)std::min(generator->scaled_raw_noise_2d(17, 35, (i + xPos * chunkSize) / 100.0f, (j + yPos * chunkSize) / 100.0f), float(worldHeightDataUpper[i][j]));
            worldHeightDataLower[i].push_back(uint16_t(generator->scaled_raw_noise_2d(12, worldHeightDataUpper[i][j] - 1, (i + xPos * chunkSize) / 10.0f, (j + yPos * chunkSize) / 10.0f)));

            bool hole = int(generator->scaled_raw_noise_2d(0, 5, (i + xPos * chunkSize) / 80.0f, (j + yPos * chunkSize) / 80.0f)) % 2 == 0;
            //hole = int(scaled_raw_noise_2d(0, 3, (i + xPos * chunkSize) / 80.0f, (j + yPos * chunkSize) / 80.0f)) % 2 == 0;
            if(hole) worldHeightDataUpper[i][j] = 0;
        }
    }

    for(uint16_t i = 0; i < chunkSize; i++) {
        for(uint16_t j = 0; j < chunkSize; j++) {
            for(uint16_t k = worldHeightDataLower[i][j]; k <= worldHeightDataUpper[i][j]; k++) {
                addCube(i, k - loadingAnimationHeight, j);
                blockHeights.push_back(k);
            }
        }
    }

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_DYNAMIC_DRAW);

    startLoadTick = loadTick = SDL_GetTicks();
    loadingDone = false;
    loaded = true;
}

void Chunk::addCube(uint16_t x, uint16_t y, uint16_t z) {
    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    triangleCount += 6;


    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    triangleCount += 6;


    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    triangleCount += 6;


    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x - 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    triangleCount += 6;


    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z - 0.5f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back((GLfloat)x + 0.5f);
    vertices.push_back((GLfloat)y - 1);
    vertices.push_back((GLfloat)z + 0.5f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    triangleCount += 6;
}

GLint Chunk::getTriangleCount() {
    return triangleCount;
}

bool Chunk::isLoaded() {
    return loaded > 0;
}
