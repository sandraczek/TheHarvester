#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <map>

class AIState;

struct Transform {
	sf::Vector2f position = { 0.f, 0.f };
	sf::Vector2f scale = { 1.f, 1.f };
	float rotation = 0.f; // W stopniach (degrees)
};
struct Movement {
	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f acceleration = { 0.f, 0.f };
	float maxSpeed = 400.f;
	float groundFriction = 0.9f;
	float airFriction = 0.98f;
};
struct Gravity {
	bool onGround = false;
};
struct Health {
	float current = 0.f;
	float max = 0.f;
	bool alive = true;
};
struct Damage {
	float damage = 0.f;
	float knockback = 0.f;
};
struct Collider {
	sf::FloatRect bounds;
	sf::Vector2f offset = { 0.f,0.f };
	int layer = -2;
	bool active = true;
};
struct AI {
	std::shared_ptr<AIState> currentState;
};
struct Input {

};
struct InputState {
	float moveDirection = 0.f; // -1.0 dla lewo, 1.0 dla prawo, 0.0 dla bezruchu
	bool wantsToJump = false;
};
	struct AnimationData {
		int startFrame = 0;
		int frameCount = 1;
		float duration = 0.1f;
	};
struct AnimationState {
	std::map<std::string, AnimationData> animations;
	std::string currentState = "idle";
	float timer = 0.f;
	int currentFrame = 0;
};
struct Renderable {
	std::string textureName;
	sf::IntRect textureRect;
 	int zIndex = 0;

	sf::Vector2i frameSize = { 32,32 };

	const sf::Texture* texture = nullptr;
};

// tags
struct EnemyTag{};
struct PlayerTag {};
struct SolidTag {};