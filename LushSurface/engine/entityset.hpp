#include "../pch.hpp"
#include "entity.hpp"

class EntitySet {
public:
    EntitySet();
    ~EntitySet();

private:
    unsigned long entityID = 0;
    std::vector<Entity> entities;
};
