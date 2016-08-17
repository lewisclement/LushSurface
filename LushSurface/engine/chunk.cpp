#include "world.hpp"
#include "generator.hpp"

Chunk::Chunk(unsigned int seed) {
    this->seed = seed;
}

Chunk::~Chunk() {
    loaded = false;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    worldHeightDataLower.clear();
    worldHeightDataUpper.clear();
    vertices.clear();
    blockHeights.clear();

    delete mesh;
    delete collisionShape;
    delete rigidBody;
}

void Chunk::bindVBO() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

void Chunk::initialize(int32_t x, int32_t y, btDiscreteDynamicsWorld *dynamicsWorld) {
    if(loaded) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);

        vertices.clear();
        blockHeights.clear();
        worldHeightDataLower.clear();
        worldHeightDataUpper.clear();

        loaded = false;
        pointCount = 0;

        dynamicsWorld->removeRigidBody(rigidBody);
        delete rigidBody->getMotionState();
        delete rigidBody;
        delete mesh;
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

    generateChunkv5();

    mesh = new btTriangleMesh();
    fillVertexes();

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_DYNAMIC_DRAW);

    collisionShape = new btBvhTriangleMeshShape{mesh, true};
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(x*chunkSize, 0, y*chunkSize)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, collisionShape, btVector3(0, 0, 0));
    groundRigidBodyCI.m_linearDamping = 100;
    groundRigidBodyCI.m_angularDamping = 100;
    groundRigidBodyCI.m_friction = 1;
    groundRigidBodyCI.m_rollingFriction = 0.4f;
    rigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(rigidBody);

    loaded = true;
}

void Chunk::generateChunkv1() {
    for(uint16_t i = 0; i < chunkSize; i++) {
        std::vector<uint16_t> subUp;
        worldHeightDataUpper.push_back(subUp);
        std::vector<uint16_t> subDown;
        worldHeightDataLower.push_back(subDown);

        for(uint16_t j = 0; j < chunkSize; j++) {
            worldHeightDataUpper[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 17, 25, (i + xPos * chunkSize) / 40.0f, (j + yPos * chunkSize) / 40.0f)));
            worldHeightDataUpper[i][j] = (uint16_t)std::max(Generate::generateValue(Generate::originalSimplex, seed, 17, 23, (i + xPos * chunkSize) / 10.0f, (j + yPos * chunkSize) / 10.0f), float(worldHeightDataUpper[i][j]));
            worldHeightDataUpper[i][j] = (uint16_t)std::min(Generate::generateValue(Generate::originalSimplex, seed, 17, 35, (i + xPos * chunkSize) / 100.0f, (j + yPos * chunkSize) / 100.0f), float(worldHeightDataUpper[i][j]));
            worldHeightDataLower[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 12, worldHeightDataUpper[i][j] - 1, (i + xPos * chunkSize) / 10.0f, (j + yPos * chunkSize) / 10.0f)));

            bool hole = int(Generate::generateValue(Generate::originalSimplex, seed, 0, 5, (i + xPos * chunkSize) / 80.0f, (j + yPos * chunkSize) / 80.0f)) % 3 == 0;
            if(hole) worldHeightDataUpper[i][j] = 0;
        }
    }
}

void Chunk::generateChunkv2() {
    for(uint16_t i = 0; i < chunkSize; i++) {
        std::vector<uint16_t> subUp;
        worldHeightDataUpper.push_back(subUp);
        std::vector<uint16_t> subDown;
        worldHeightDataLower.push_back(subDown);

        for(uint16_t j = 0; j < chunkSize; j++) {
            worldHeightDataUpper[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 17, 25, (i + xPos * chunkSize) / 40.0f, (j + yPos * chunkSize) / 40.0f)));
            worldHeightDataUpper[i][j] = (uint16_t)std::max(Generate::generateValue(Generate::originalSimplex, seed, 17, 23, (i + xPos * chunkSize) / 10.0f, (j + yPos * chunkSize) / 10.0f), float(worldHeightDataUpper[i][j]));
            worldHeightDataUpper[i][j] = (uint16_t)std::min(Generate::generateValue(Generate::originalSimplex, seed, 17, 35, (i + xPos * chunkSize) / 100.0f, (j + yPos * chunkSize) / 100.0f), float(worldHeightDataUpper[i][j]));

            bool height = int(Generate::generateValue(Generate::originalSimplex, seed, 0, 3, (i + xPos * chunkSize) / 40.0f, (j + yPos * chunkSize) / 40.0f)) == 2;
            if(height) worldHeightDataUpper[i][j] = (uint16_t)std::max(Generate::generateValue(Generate::originalSimplex, seed, 23, 30, (i + xPos * chunkSize) / 50.0f, (j + yPos * chunkSize) / 50.0f), float(worldHeightDataUpper[i][j]));

            worldHeightDataLower[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 12, 24, (i + xPos * chunkSize) / 120.0f, (j + yPos * chunkSize) / 120.0f)));

            bool hole = int(Generate::generateValue(Generate::originalSimplex, seed, 0, 5, (i + xPos * chunkSize) / 80.0f, (j + yPos * chunkSize) / 80.0f)) == 0;
            if(hole) worldHeightDataUpper[i][j] = 0;

            if(worldHeightDataLower[i][j] >= worldHeightDataUpper[i][j]) worldHeightDataUpper[i][j] = 0;
        }
    }
}

void Chunk::generateChunkv3() {
    for(uint16_t i = 0; i < chunkSize; i++) {
        std::vector<uint16_t> subUp;
        worldHeightDataUpper.push_back(subUp);
        std::vector<uint16_t> subDown;
        worldHeightDataLower.push_back(subDown);

        for(uint16_t j = 0; j < chunkSize; j++) {
            worldHeightDataUpper[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 17, 25, (i + xPos * chunkSize) / 40.0f, (j + yPos * chunkSize) / 40.0f)));

            int layer = int(Generate::generateValue(Generate::originalSimplex, seed, 0, 3, (i + xPos * chunkSize) / 20.0f, (j + yPos * chunkSize) / 20.0f));
            if(layer == 2) {
                worldHeightDataUpper[i][j] += 8;
            } else if (layer == 1) {
                worldHeightDataUpper[i][j] += 5;
            }

            layer = int(Generate::generateValue(Generate::originalSimplex, seed, 0, 5, (i + xPos * chunkSize) / 30.0f, (j + yPos * chunkSize) / 30.0f));
            if(layer == 3) {
                worldHeightDataUpper[i][j] += 2;
            }

            layer = int(Generate::generateValue(Generate::originalSimplex, seed, 0, 15, (i + xPos * chunkSize) / 60.0f, (j + yPos * chunkSize) / 60.0f));
            if(layer == 14) {
                worldHeightDataUpper[i][j] += 11;
            }

            layer = int(Generate::generateValue(Generate::originalSimplex, seed, 0, 15, (i + xPos * chunkSize) / 40.0f, (j + yPos * chunkSize) / 40.0f));
            if(layer > 11 || layer < 5) {
                worldHeightDataUpper[i][j] = 0;
            }

            layer = int(Generate::generateValue(Generate::originalSimplex, seed, 0, 8, (i + xPos * chunkSize) / 400.0f, (j + yPos * chunkSize) / 400.0f));
            if(worldHeightDataUpper[i][j] != 0) {
                worldHeightDataUpper[i][j] -= layer;
            }


            if(worldHeightDataUpper[i][j] == 0) {
                worldHeightDataLower[i].push_back(0);
            } else
                worldHeightDataLower[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 12, worldHeightDataUpper[i][j] - 1, (i + xPos * chunkSize) / 20.0f, (j + yPos * chunkSize) / 20.0f)));


            if(worldHeightDataLower[i][j] >= worldHeightDataUpper[i][j]) worldHeightDataUpper[i][j] = 0;
        }
    }
}

void Chunk::generateChunkv4() {
    for(uint16_t i = 0; i < chunkSize; i++) {
        std::vector<uint16_t> subUp;
        worldHeightDataUpper.push_back(subUp);
        std::vector<uint16_t> subDown;
        worldHeightDataLower.push_back(subDown);

        for(uint16_t j = 0; j < chunkSize; j++) {
            worldHeightDataUpper[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 17, 25, (i + xPos * chunkSize) / 400.0f, (j + yPos * chunkSize) / 400.0f)));
            uint16_t layer = uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 1, 50, (i + xPos * chunkSize) / 20.0f, (j + yPos * chunkSize) / 20.0f));
            uint16_t holeSensitivity = uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 0, 10, (i + xPos * chunkSize) / 50.0f, (j + yPos * chunkSize) / 50.0f));

            if(layer >= 20 && layer <= 25)
                worldHeightDataUpper[i][j] = layer;
            else if (layer > 25)
                worldHeightDataUpper[i][j] = uint16_t(30 + Generate::generateValue(Generate::originalSimplex, seed, 0, 5, (i + xPos * chunkSize) / 10.0f, (j + yPos * chunkSize) / 10.0f));
            else if (layer < 10 + holeSensitivity)
                worldHeightDataUpper[i][j] = 0;

            layer = uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 0, 50, (i + xPos * chunkSize) / 80.0f, (j + yPos * chunkSize) / 80.0f));
            if(layer > 20)
                worldHeightDataUpper[i][j] -= layer-20;

            if(worldHeightDataUpper[i][j] == 0) {
                worldHeightDataLower[i].push_back(0);
            } else
                worldHeightDataLower[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 12, worldHeightDataUpper[i][j] - 1, (i + xPos * chunkSize) / 20.0f, (j + yPos * chunkSize) / 20.0f)));


            if(worldHeightDataLower[i][j] >= worldHeightDataUpper[i][j]) worldHeightDataUpper[i][j] = 0;
            if(worldHeightDataUpper[i][j] > 50) worldHeightDataUpper[i][j] = 0;
        }
    }
}

void Chunk::generateChunkv5() {
    for(uint16_t i = 0; i < chunkSize; i++) {
        std::vector<uint16_t> subUp;
        worldHeightDataUpper.push_back(subUp);
        std::vector<uint16_t> subDown;
        worldHeightDataLower.push_back(subDown);

        for(uint16_t j = 0; j < chunkSize; j++) {
            worldHeightDataUpper[i].push_back(0);
            uint16_t layer = uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 1, 50, (i + xPos * chunkSize) / 50.0f, (j + yPos * chunkSize) / 50.0f));
            uint16_t holeSensitivity = uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 0, 10, (i + xPos * chunkSize) / 50.0f, (j + yPos * chunkSize) / 50.0f));

            if(layer >= 20 && layer <= 25)
                worldHeightDataUpper[i][j] = layer;
            else if (layer > 25) {
                float diff = (layer - 25) / 10.0f;

                worldHeightDataUpper[i][j] = uint16_t(25 + diff * Generate::generateValue(Generate::originalSimplex, seed, 0, 5, (i + xPos * chunkSize) / 10.0f, (j + yPos * chunkSize) / 10.0f));
            } else if (layer < 10 + holeSensitivity)
                worldHeightDataUpper[i][j] = 0;
            else if (layer >= 10 + holeSensitivity && layer < 20) {
                worldHeightDataUpper[i][j] = 20;
            }

            if(worldHeightDataUpper[i][j] == 25) {
                worldHeightDataUpper[i][j] += uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 0, 3, (i + xPos * chunkSize) / 20.0f, (j + yPos * chunkSize) / 20.0f));
            }

            if(worldHeightDataUpper[i][j] == 0) {
                worldHeightDataLower[i].push_back(0);
            } else
                worldHeightDataLower[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 12, worldHeightDataUpper[i][j] - 1, (i + xPos * chunkSize) / 20.0f, (j + yPos * chunkSize) / 20.0f)));


            if(worldHeightDataLower[i][j] >= worldHeightDataUpper[i][j]) worldHeightDataUpper[i][j] = 0;
            if(worldHeightDataUpper[i][j] > 50) worldHeightDataUpper[i][j] = 0;
        }
    }
}

void Chunk::generateChunkv6() {
    for(uint16_t i = 0; i < chunkSize; i++) {
        std::vector<uint16_t> subUp;
        worldHeightDataUpper.push_back(subUp);
        std::vector<uint16_t> subDown;
        worldHeightDataLower.push_back(subDown);

        for(uint16_t j = 0; j < chunkSize; j++) {
            worldHeightDataUpper[i].push_back(0);
            uint16_t layer = uint16_t(Generate::generateValue(Generate::FBMWarpedFlow, seed, 1, 50, (i + xPos * chunkSize) / 50.0f, (j + yPos * chunkSize) / 50.0f));
            uint16_t holeSensitivity = uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 0, 10, (i + xPos * chunkSize) / 50.0f, (j + yPos * chunkSize) / 50.0f));

            if(layer >= 20 && layer <= 25)
                worldHeightDataUpper[i][j] = layer;
            else if (layer > 25) {
                float diff = (layer - 25) / 10.0f;

                worldHeightDataUpper[i][j] = uint16_t(25 + diff * Generate::generateValue(Generate::originalSimplex, seed, 0, 5, (i + xPos * chunkSize) / 10.0f, (j + yPos * chunkSize) / 10.0f));
            } else if (layer < 10 + holeSensitivity)
                worldHeightDataUpper[i][j] = 0;
            else if (layer >= 10 + holeSensitivity && layer < 20) {
                worldHeightDataUpper[i][j] = 20;
            }

            if(worldHeightDataUpper[i][j] == 25) {
                worldHeightDataUpper[i][j] += uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 0, 3, (i + xPos * chunkSize) / 20.0f, (j + yPos * chunkSize) / 20.0f));
            }

            if(worldHeightDataUpper[i][j] == 0) {
                worldHeightDataLower[i].push_back(0);
            } else
                worldHeightDataLower[i].push_back(uint16_t(Generate::generateValue(Generate::originalSimplex, seed, 12, worldHeightDataUpper[i][j] - 1, (i + xPos * chunkSize) / 20.0f, (j + yPos * chunkSize) / 20.0f)));


            if(worldHeightDataLower[i][j] >= worldHeightDataUpper[i][j]) worldHeightDataUpper[i][j] = 0;
            if(worldHeightDataUpper[i][j] > 50) worldHeightDataUpper[i][j] = 0;
        }
    }
}

void Chunk::fillVertexes() {
    for(uint16_t x = 0; x < chunkSize; x++) {
        for(uint16_t z = 0; z < chunkSize; z++) {
            if(worldHeightDataUpper[x][z] == 0) continue;
            for(uint16_t y = worldHeightDataUpper[x][z]; y >= worldHeightDataLower[x][z]; y--) {
                if(y == worldHeightDataUpper[x][z]) {
                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector1{x, y, z};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector2{x, y, z + 1};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector3{x + 1, y, z};

                    mesh->addTriangle(vector1, vector2, vector3);

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector4{x + 1, y, z};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector5{x, y, z + 1};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector6{x + 1, y, z + 1};
                    mesh->addTriangle(vector4, vector5, vector6);
                    pointCount += 6;
                }

                if(x == 0 || y > worldHeightDataUpper[x-1][z] || y < worldHeightDataLower[x-1][z]) {
                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(-1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector1{x, y, z + 1};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(-1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector2{x, y, z};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(-1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector3{x, y - 1, z};
                    mesh->addTriangle(vector1, vector2, vector3);

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(-1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector4{x, y, z + 1};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(-1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector5{x, y - 1, z};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(-1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector6{x, y - 1, z + 1};
                    mesh->addTriangle(vector4, vector5, vector6);
                    pointCount += 6;
                }

                if(x == chunkSize-1 || y > worldHeightDataUpper[x+1][z] || y < worldHeightDataLower[x+1][z]) {
                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector1{x + 1, y, z};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector2{x + 1, y, z + 1};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector3{x + 1, y - 1, z};
                    mesh->addTriangle(vector1, vector2, vector3);

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector4{x + 1, y - 1, z};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector5{x + 1, y, z + 1};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    btVector3 vector6{x + 1, y - 1, z + 1};
                    mesh->addTriangle(vector4, vector5, vector6);
                    pointCount += 6;
                }

                if(z == 0 || y > worldHeightDataUpper[x][z-1] || y < worldHeightDataLower[x][z-1]) {
                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(-1.0f);
                    btVector3 vector1{x, y, z};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(-1.0f);
                    btVector3 vector2{x + 1, y, z};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(-1.0f);
                    btVector3 vector3{x, y - 1, z};
                    mesh->addTriangle(vector1, vector2, vector3);

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(-1.0f);
                    btVector3 vector4{x, y - 1, z};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(-1.0f);
                    btVector3 vector5{x + 1, y, z};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(-1.0f);
                    btVector3 vector6{x + 1, y - 1, z};
                    mesh->addTriangle(vector4, vector5, vector6);
                    pointCount += 6;
                }

                if(z == chunkSize-1 || y > worldHeightDataUpper[x][z+1] || y < worldHeightDataLower[x][z+1]) {
                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    btVector3 vector1{x + 1, y, z + 1};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    btVector3 vector2{x, y, z + 1};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    btVector3 vector3{x, y - 1, z + 1};
                    mesh->addTriangle(vector1, vector2, vector3);

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    btVector3 vector4{x + 1, y, z + 1};

                    vertices.push_back((GLfloat)x);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    btVector3 vector5{x, y - 1, z + 1};

                    vertices.push_back((GLfloat)x + 1);
                    vertices.push_back((GLfloat)y - 1);
                    vertices.push_back((GLfloat)z + 1);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    btVector3 vector6{x + 1, y - 1, z + 1};
                    mesh->addTriangle(vector4, vector5, vector6);
                    pointCount += 6;
                }
            }
        }
    }
}

GLint Chunk::getTriangleCount() {
    return pointCount;
}

bool Chunk::isLoaded() {
    return loaded > 0;
}
