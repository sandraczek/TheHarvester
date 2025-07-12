#pragma once
#include "entt/entt.hpp"
#include "Resources.hpp"
#include "Component.hpp"

//SFML graphics + audio

struct CollisionProxy {
    entt::entity entity;
    sf::FloatRect bounds;

    sf::Vector2f getPosition() const {
        return bounds.position;
    }

    sf::FloatRect getGlobalBounds() const {
        return bounds;
    }
};

struct Context {
    bool debugMode = false;

    Resources resources;
    entt::registry registry;
    entt::dispatcher dispatcher{};

   
};