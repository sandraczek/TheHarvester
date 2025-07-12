#pragma once
#include "Context.hpp"

class AnimationSystem {
public:
    AnimationSystem(Context& C) : C(C) {}
    void update(float dTime);
private:
    Context& C;
};