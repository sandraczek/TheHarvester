#pragma once
#include "Scene.hpp"
#include "Factory.h"

// Systems
#include "DamageSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "AnimationSystem.h"
#include "CollisionSystem.h"
#include "UserInputSystem.h"

class GameScene : public Scene
{
private:
    Context& C;
    Factory spawn;
    struct {
        RenderSystem render;
        MovementSystem movement;
        AnimationSystem animation;
        CollisionSystem collisions;
        DamageSystem damage;
        UserInputSystem userInput;
    } systems;
public:
    GameScene(Context& C, sf::RenderWindow& window);
    ~GameScene();

    void handleEvents(const sf::Event& event) override;
    void update(float dTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};


