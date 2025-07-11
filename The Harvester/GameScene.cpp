#include "GameScene.h"
//#include "Game.h"
#include "EntityFactory.hpp"
#include "MapFactory.hpp"

GameScene::GameScene(Context& C, sf::RenderWindow& window) : systems{ RenderSystem(window.getSize()) } {
    spawnFloor(C, { {-1000.f, 1000.f}, {4000.f, 200.f} });
    spawnPlayer(C, { 100.f,100.f });
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
    systems.ai.update(C, dTime);
    systems.movement.update(C, dTime);
    systems.collisions.update(C);
	systems.animation.update(C, dTime);
}
void GameScene::draw(Context& C, sf::RenderWindow& window) {
    systems.render.draw(C, window);
}