#include "../pch.hpp"
#include "../geometry.hpp"

class Entity {
public:
    Entity(unsigned long ID);
    ~Entity();

private:
    unsigned long ID = 0;
    Point location;
};
