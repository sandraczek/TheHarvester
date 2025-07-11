#pragma once
#include "Context.hpp"
#include <memory>

enum class AIStateType {
	Idle,
	Patrol,
	Chase
};

class AIState {
public:
    virtual ~AIState() = default;

    virtual void enter(Context& C, entt::entity e) = 0;

    virtual std::shared_ptr<AIState> update(Context& C, entt::entity e, float deltaTime) = 0;

    virtual void exit(Context& C, entt::entity e) = 0;

    virtual AIStateType getType() const = 0;
};