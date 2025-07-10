#include "RenderSystem.h"
#include "Component.hpp"
#include <iostream>

constexpr sf::Color backgroundColor = { 20,20,40,255 };

struct RenderJob {
	int zIndex;
	float y_pos;
	sf::Vector2f position;
	sf::IntRect texRect;
	const sf::Texture* texture;

	bool operator<(const RenderJob& other) const {
		if (zIndex != other.zIndex) {
			return zIndex < other.zIndex;
		}
		return y_pos < other.y_pos;
	}
};

RenderSystem::RenderSystem(sf::Vector2u windowSize) {
	camera.setSize(static_cast<sf::Vector2f>(windowSize));
	camera.setCenter({ windowSize.x / 2.f, windowSize.y / 2.f });
}

void RenderSystem::draw(Context& C, sf::RenderWindow& window, float dTime) {
	for (auto [entity, transform, renderable] : C.components.view<Transform, PlayerTag>()) {
		camera.setCenter(transform.position);
	}
	window.setView(camera);

	std::vector<RenderJob> renderQueue;

	for (auto [entity, transform, renderable] : C.components.view<Transform, Renderable>()) {
		if (!renderable.texture) {
			renderable.texture = &C.resources.getTexture(renderable.textureName);
		}
		const auto* tex = renderable.texture;
		if (!tex) continue;

		if (auto* anim = C.components.try_get<AnimationState>(entity)) {
			anim->timer += dTime;
			const auto& animData = anim->animations.at(anim->currentState);

			if (anim->timer >= animData.duration) {
				anim->timer = 0.f;
				anim->currentFrame = (anim->currentFrame + 1) % animData.frameCount;
				
			}

			int frameX = (animData.startFrame + anim->currentFrame) * renderable.frameSize.x;
			int frameY = 0; // Zak³adamy, ¿e wszystkie klatki s¹ w jednym rzêdzie
			renderable.textureRect = { { frameX, frameY }, { renderable.frameSize.x, renderable.frameSize.y } };
		}

		renderQueue.push_back({
			renderable.zIndex,
			transform.position.y,
			transform.position,
			renderable.textureRect,
			renderable.texture
			});
	}

	std::sort(renderQueue.begin(), renderQueue.end());

	window.clear(backgroundColor);

	sf::VertexArray batch(sf::PrimitiveType::Triangles);
	const sf::Texture* currentTexture = nullptr;

	for (const auto& job : renderQueue) {
		if (job.texture != currentTexture) {
			if (!batch.getVertexCount() == 0) {
				sf::RenderStates states;
				states.texture = currentTexture;
				window.draw(batch, states);
				batch.clear();
			}
			currentTexture = job.texture;
		}

		const sf::Vector2f& pos = job.position;
		const sf::IntRect& rect = job.texRect;
		const sf::Vector2f size = static_cast<sf::Vector2f>(rect.size);

		sf::Vector2f texTL = { static_cast<float>(rect.position.x), static_cast<float>(rect.position.x) };
		sf::Vector2f texTR = { static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y) };
		sf::Vector2f texBL = { static_cast<float>(rect.position.x), static_cast<float>(rect.position.y + rect.size.y) };
		sf::Vector2f texBR = { static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y + rect.size.y) };

		batch.append(sf::Vertex{ pos + sf::Vector2f{ 0.f,0.f },  });
		batch.append(sf::Vertex{ pos + sf::Vector2f{ 0.f,size.y } });
		batch.append(sf::Vertex{ pos + sf::Vector2f{ size.x,0.f } });
		batch.append(sf::Vertex{ pos + sf::Vector2f{ size.x,0.f } });
		batch.append(sf::Vertex{ pos + sf::Vector2f{ 0.f,size.y } });
		batch.append(sf::Vertex{ pos + sf::Vector2f{ size.x,size.y } });

		if (!batch.getVertexCount() == 0 && currentTexture) {
			sf::RenderStates states;
			states.texture = currentTexture;
			window.draw(batch, states);
		}
	}

	// Rysowanie HUD/UI by³oby tutaj, po zresetowaniu widoku do domyœlnego
	// window.setView(window.getDefaultView());
	// ... rysowanie UI ...
}