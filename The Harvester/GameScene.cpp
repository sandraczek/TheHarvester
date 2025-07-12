#include "GameScene.h"
#include "Factory.h"

GameScene::GameScene(Context& C, sf::RenderWindow& window) :C(C), spawn(C), systems{ RenderSystem(C, window.getSize()), MovementSystem(C), AnimationSystem(C), CollisionSystem(C), DamageSystem(C), UserInputSystem(C)} {
    spawn.Box({ {-1000.f, 1000.f}, {4000.f, 200.f} });
    spawn.Box({ {200.f, 600.f}, {100.f, 100.f} });
    spawn.Box({ {400.f, 900.f}, {100.f, 100.f} });
    spawn.Player({ 100.f,100.f });
}
GameScene::~GameScene() {}

void GameScene::handleEvents(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::LBracket) {
            spawn.Spider({ 0.f,0.f });
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
void GameScene::update(float dTime, sf::RenderWindow& window) {
    systems.userInput.update();
    systems.movement.update(dTime);
    systems.collisions.update();

    systems.damage.update();
	systems.animation.update(dTime);
}
void GameScene::draw(sf::RenderWindow& window) {
    systems.render.draw(window);
}