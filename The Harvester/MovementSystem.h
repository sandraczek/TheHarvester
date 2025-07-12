#pragma once
#include "Context.hpp"

class CollisionEvent;

class MovementSystem
{
public:
	MovementSystem(Context& C);
	void update(float dTime);

	void onCollisionEvent(const CollisionEvent& event);
private:
	Context& C;
};

