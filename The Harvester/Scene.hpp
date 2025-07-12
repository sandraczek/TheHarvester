#pragma once
#include <SFML/Graphics.hpp>

struct Context;

class Scene {
public:
    virtual ~Scene() = default;
    virtual void handleEvents(const sf::Event& event) = 0;
    virtual void update(float dTime, sf::RenderWindow& window) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};