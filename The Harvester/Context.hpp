#pragma once
#include "entt/entt.hpp"
#include "Resources.hpp"
#include "Quadtree.hpp"
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
    //QuadTree<CollisionProxy> qtree;
    static constexpr float MAP_WIDTH = 1000.0f;
    static constexpr float MAP_HEIGHT = 800.0f;
    static constexpr size_t maxElements = 4;
    static constexpr size_t maxDepth = 8;
    bool debugMode = false;

    Resources resources;
    entt::registry registry;

   
};