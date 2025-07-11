#include "RenderSystem.h"

constexpr sf::Color backgroundColor = { 20,20,40,255 };

struct RenderJob {
	int zIndex;
	sf::Vector2f position;
    sf::Vector2f scale;
	sf::IntRect texRect;
	const sf::Texture* texture;

	bool operator<(const RenderJob& other) const {
		if (zIndex != other.zIndex) {
			return zIndex < other.zIndex;
		}
		return position.y < other.position.y;
	}
};

RenderSystem::RenderSystem(sf::Vector2u windowSize) {
	camera.setSize(static_cast<sf::Vector2f>(windowSize));
	camera.setCenter({ windowSize.x / 2.f, windowSize.y / 2.f });
}

void RenderSystem::draw(Context& C, sf::RenderWindow& window) {
    for (const auto& [entity, transform] : C.registry.view<Transform, PlayerTag>().each()) {
        camera.setCenter(transform.position);
    }

    window.setView(camera);

    std::vector<RenderJob> renderQueue;

    for (auto [entity, transform, renderable] : C.registry.view<Transform, Renderable>().each()) {
        if (!renderable.texture) {
            renderable.texture = &C.resources.getTexture(renderable.textureName);
        }
        if (!renderable.texture) continue;

        renderQueue.push_back({
            renderable.zIndex,
            transform.position,
            transform.scale,
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
            if (batch.getVertexCount() > 0) {
                sf::RenderStates states;
                states.texture = currentTexture;
                window.draw(batch, states);
                batch.clear();
            }
            currentTexture = job.texture;
        }

        // Pobranie danych do renderowania
        const sf::Vector2f& pos = job.position;
        const sf::IntRect& rect = job.texRect;
        const sf::Vector2f size = {
            static_cast<float>(rect.size.x) * job.scale.x,
            static_cast<float>(rect.size.y) * job.scale.y
        };

        // Obliczenie wspó³rzêdnych wierzcho³ków i tekstury
        sf::Vertex v[4];
        v[0].position = pos;
        v[1].position = pos + sf::Vector2f(size.x, 0.f);
        v[2].position = pos + sf::Vector2f(size.x, size.y);
        v[3].position = pos + sf::Vector2f(0.f, size.y);

        v[0].texCoords = sf::Vector2f(static_cast<float>(rect.position.x), static_cast<float>(rect.position.y));
        v[1].texCoords = sf::Vector2f(static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y));
        v[2].texCoords = sf::Vector2f(static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y + rect.size.y));
        v[3].texCoords = sf::Vector2f(static_cast<float>(rect.position.x), static_cast<float>(rect.position.y + rect.size.y));

        batch.append(v[0]);
        batch.append(v[1]);
        batch.append(v[3]);

        batch.append(v[1]);
        batch.append(v[2]);
        batch.append(v[3]);
    }

    // Narysuj ostatni batch, który pozosta³ w pêtli
    if (batch.getVertexCount() > 0 && currentTexture) {
        sf::RenderStates states;
        states.texture = currentTexture;
        window.draw(batch, states);
    }

    for (const auto& [e, coll, render] : C.registry.view<Collider, Renderable>().each()) {
        sf::RectangleShape rect;
        rect.setPosition(coll.bounds.position);
        rect.setSize(coll.bounds.size);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineThickness(1);
        rect.setOutlineColor(sf::Color::Red);
        window.draw(rect);
    }

    //C.qtree.draw(&window);
    //C.qtree.reset();
}