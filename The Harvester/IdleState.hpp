#pragma once
#include "baseState.hpp"
#include "Component.hpp"

class IdleState : public AIState {
public:
    AIStateType getType() const override { return AIStateType::Idle; }
    void enter(Context& C, Entity e) override {
        if (C.components.has<Movement>(e)) {
            C.components.get<Movement>(e).velocity = { 0.f, 0.f };
        }
    }

    std::shared_ptr<AIState> update(Context& C, Entity e, float deltaTime) override {
        if (C.components.has<Transform>(e)) {
            const sf::Vector2f& enemyPos = C.components.get<Transform>(e).position;

            //float distance = std::hypot(enemyPos.x - playerPos.x, enemyPos.y - playerPos.y);
           // if (distance < 300.f) {
              //  return std::make_shared<PatrolState>();
           // }
        }
        return nullptr;
    }

    void exit(Context& C, Entity e) override {
    }
};