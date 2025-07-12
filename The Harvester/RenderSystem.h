#pragma once
#include "Context.hpp"

class Context;

class RenderSystem
{
private:
	sf::View camera;
	Context& C;
public:
	RenderSystem(Context& C, sf::Vector2u windowSize);
	void draw(sf::RenderWindow& window);
};

