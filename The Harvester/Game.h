#pragma once
#include "Context.hpp"
#include "Resources.hpp"

//


class Scene;

class Game
{
public:
	Context C;
	std::unique_ptr<Scene> currentScene;
	sf::RenderWindow window;
	sf::Clock clock;
	bool isPaused = false;
	

public:
	Game();
	~Game();

	void run();

	void changeScene(std::unique_ptr<Scene> newScene);
	sf::RenderWindow& getWindow() { return window; };
private:
	void handleEvents();
	void update(float dt);
	void render();
	void initialize();
};

