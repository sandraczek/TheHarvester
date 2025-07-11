#pragma once
#include "Context.hpp"
#include <SFML/Graphics.hpp>

void spawnFloor(Context& C, const sf::FloatRect& rect) {
    const sf::IntRect textureRect = { {0,0}, { 4, 4 } };

    auto e = C.registry.create();
    C.registry.emplace<Transform>(e, rect.position, sf::Vector2f{rect.size.x / static_cast<float>(textureRect.size.x), rect.size.y / static_cast<float>(textureRect.size.y)});
    C.registry.emplace<SolidTag>(e);
    C.registry.emplace<Renderable>(e, "floor", textureRect, -10);
    C.registry.emplace<Collider>(e, sf::FloatRect{{0.f,0.f}, rect.size}, sf::Vector2f{0.f,0.f}, -1);
}