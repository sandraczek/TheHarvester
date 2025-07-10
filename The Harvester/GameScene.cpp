#include "GameScene.h"
//#include "Game.h"
#include "Factory.hpp"

GameScene::GameScene(Context& C, sf::RenderWindow& window): renderSys(window.getSize())  {
    floor.setFillColor(sf::Color::Black);
    floor.setSize({ static_cast<float>(window.getSize().x + 2000),200.f });
    floor.setPosition({ -1000,1000 });
    spawnPlayer(C, { 0.f,0.f });
}
GameScene::~GameScene() {}

void GameScene::handleEvents(Context& C, const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::LBracket) {
            spawnSpider(C, { 0.f,0.f });
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::RBracket) {
            //makeEntity<Turtle>();
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Backslash) {
            //makeEntity<Dragon>();
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::F1) {
            C.debugMode = !C.debugMode;
        }
    }
}
void GameScene::update(Context& C, float dTime, sf::RenderWindow& window) {
    ai.update(C, dTime);
    movement.update(C, dTime);
    renderSys.draw(C, window, dTime);
}
void GameScene::render(Context& C, sf::RenderWindow& window) {
    window.draw(floor);
}