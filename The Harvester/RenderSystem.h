#pragma once
#include "Context.hpp"

class Context;

class RenderSystem
{
private:
	sf::View camera;
public:
	RenderSystem(sf::Vector2u windowSize);
	void draw(Context& C, sf::RenderWindow& window);
};

