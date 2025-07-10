#include "Game.h"

//Scenes
#include "GameScene.h"

Game::Game() : window(sf::VideoMode({ 3840, 2160 }), "The Harvester", sf::State::Fullscreen){
    C.resources.loadFont("regular", "assets/font/SchibstedGrotesk-Regular.ttf");
    C.resources.loadFont("medium", "assets/font/SchibstedGrotesk-Medium.ttf");
    C.resources.loadTexture("player", "assets/player/player/player-spritemap-v9.png");
    C.resources.loadTexture("critters", "assets/critters.png");
    C.resources.loadTexture("bosses", "assets/bosses.png");

    window.setFramerateLimit(144);
    changeScene(std::make_unique<GameScene>(C, window));
    initialize();

}
Game::~Game() {}

void Game::initialize() {
    srand(static_cast<unsigned int>(time(0)));
    clock.restart();
}
void Game::run() {
    while (window.isOpen()) {
        const float dTime = clock.restart().asSeconds();
        handleEvents();
        if (!isPaused)
            update(dTime);
        render();
    }
}
void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {

        if (event->is<sf::Event::FocusLost>()) {
            isPaused = true;
        }

        if (event->is<sf::Event::FocusGained>()) {
            isPaused = false;
        }

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                window.close();
            }
        }

        if (currentScene && !isPaused) {
            currentScene->handleEvents(C, *event);
        }
    }
}
void Game::changeScene(std::unique_ptr<Scene> newScene) {
    if (newScene) {
        currentScene = std::move(newScene);
    }
}
void Game::update(float dTime) {
    if (currentScene) {
        currentScene->update(C, dTime, window);
    }
}
void Game::render() {
    
    if (currentScene) {
        currentScene->render(C, window);
    }
    window.display();
}