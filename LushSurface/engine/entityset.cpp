#include "entityset.hpp"

EntitySet::EntitySet() {

}

EntitySet::~EntitySet() {
    entities.clear();
}

void EntitySet::Initialize() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}
