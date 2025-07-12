#pragma once
#include "Context.hpp"

struct CollisionEvent {
    entt::entity e1;
    entt::entity e2;
    sf::FloatRect intersection;
};