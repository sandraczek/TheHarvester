#pragma once
#include "Scene.hpp"
#include "AISystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "AnimationSystem.h"
#include "CollisionSystem.h"

class GameScene : public Scene
{
private:
    struct {
        RenderSystem render;
        AISystem ai;
        MovementSystem movement;
        AnimationSystem animation;
        CollisionSystem collisions;
    } systems;
public:
    GameScene(Context& C, sf::RenderWindow& window);
    ~GameScene();

    void handleEvents(Context& C, const sf::Event& event) override;
    void update(Context& C, float dTime, sf::RenderWindow& window) override;
    void draw(Context& C, sf::RenderWindow& window) override;
};


