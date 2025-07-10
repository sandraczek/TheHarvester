#pragma once
#include "baseState.hpp"
#include "Component.hpp"

class PatrolState : public AIState {
public:
    AIStateType getType() const override { return AIStateType::Idle; }
    void enter(Context& C, Entity e) override {
        auto* input = C.components.try_get<InputState>(e);
        input->moveDirection = 0.f;
    }

    std::shared_ptr<AIState> update(Context& C, Entity e, float deltaTime) override {
        auto* input = C.components.try_get<InputState>(e);
        input->moveDirection = 1.f;
        if (rand() % 1000 < 100)
            input->wantsToJump = true;
    }

    void exit(Context& C, Entity e) override {
    }
};