#pragma once
#include "Scene.hpp"
#include "AISystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"

class GameScene : public Scene
{
private:
    sf::RectangleShape floor; //-------------to change

    //
    AISystem ai;
    MovementSystem movement;
    RenderSystem renderSys;
public:
    GameScene(Context& C, sf::RenderWindow& window);
    ~GameScene();

    void handleEvents(Context& C, const sf::Event& event) override;
    void update(Context& C, float dTime, sf::RenderWindow& window) override;
    void render(Context& C, sf::RenderWindow& window) override;
};


