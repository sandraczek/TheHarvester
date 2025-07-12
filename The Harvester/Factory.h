#pragma once
#include "Context.hpp"

class Factory
{
public:
	Factory(Context& C);
	~Factory() = default;
	entt::entity Player(const sf::Vector2f& pos);
	entt::entity Spider(const sf::Vector2f& pos);
	entt::entity Box(const sf::FloatRect& rect);
private:
	Context& C;
};

