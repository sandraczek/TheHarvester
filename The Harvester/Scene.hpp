#pragma once
#include <SFML/Graphics.hpp>

struct Context;

class Scene {
public:
    virtual ~Scene() = default;
    virtual void handleEvents(Context& C, const sf::Event& event) = 0;
    virtual void update(Context& C, float dTime, sf::RenderWindow& window) = 0;
    virtual void draw(Context& C, sf::RenderWindow& window) = 0;
};