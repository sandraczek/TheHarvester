#pragma once
#include "Event.hpp"

class DamageSystem {
public:
    DamageSystem(Context& C);
    void update();
    void onCollisionEvent(const CollisionEvent& event);
private:
    Context& C;
};


