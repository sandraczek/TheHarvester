#pragma once
#include "baseState.hpp"

class IdleState : public AIState {
public:
    AIStateType getType() const override { return AIStateType::Idle; }
    void enter(Context& C, entt::entity e) override {
        if (auto* input = C.registry.try_get<InputState>(e)) {
            input->moveDirection = 0.f;
        }
    }

    std::shared_ptr<AIState> update(Context& C, entt::entity e, float deltaTime) override {

        if (const auto& player = C.registry.all_of<PlayerTag>(e)) {

            //float distance = std::hypot(enemyPos.x - playerPos.x, enemyPos.y - playerPos.y);
           // if (distance < 300.f) {
              //  return std::make_shared<PatrolState>();
           // }
        }
        return nullptr;
    }

    void exit(Context& C, entt::entity e) override {
    }
};