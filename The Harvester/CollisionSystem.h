#pragma once
#include "Event.hpp"

class CollisionSystem {
public:
    CollisionSystem(Context& C) : C(C){}
    void update();
private:
    Context& C;
};