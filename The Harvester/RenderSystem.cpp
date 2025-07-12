#include "RenderSystem.h"
#include <iostream>

constexpr sf::Color backgroundColor = { 20,20,40,255 };

struct RenderJob {
	int zIndex;
	sf::Vector2f position;
    sf::Vector2f scale;
    sf::Angle rotation;
	sf::IntRect texRect;
	const sf::Texture* texture;
    float direction = 0.f;

	bool operator<(const RenderJob& other) const {
		if (zIndex != other.zIndex) {
			return zIndex < other.zIndex;
		}
		return position.y < other.position.y;
	}
};

RenderSystem::RenderSystem(Context& C, sf::Vector2u windowSize) :C(C){
	camera.setSize(static_cast<sf::Vector2f>(windowSize));
	camera.setCenter({ windowSize.x / 2.f, windowSize.y / 2.f });
}

void RenderSystem::draw(sf::RenderWindow& window) {
    for (auto [entity, transform] : C.registry.view<Transform, PlayerTag>().each()) {
        camera.setCenter(transform.position);
    }

    window.setView(camera);

    std::vector<RenderJob> renderQueue;

    for (auto [entity, transform, renderable] : C.registry.view<Transform, Renderable>().each()) {
        if (!renderable.texture) {
            renderable.texture = &C.resources.getTexture(renderable.textureName);
        }
        if (!renderable.texture) continue;

        RenderJob j{
            renderable.zIndex,
            transform.position,
            transform.scale,
            transform.rotation,
            renderable.textureRect,
            renderable.texture
        };
        if (auto* input = C.registry.try_get<InputState>(entity))
            j.direction = input->moveDirection;

        renderQueue.push_back(j);
    }

    std::sort(renderQueue.begin(), renderQueue.end());

    window.clear(backgroundColor);

    sf::VertexArray batch(sf::PrimitiveType::Triangles);
    const sf::Texture* currentTexture = nullptr;

    for (auto& job : renderQueue) {
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
            static_cast<float>(rect.size.x) * job.scale.x,// * 0.5f,
            static_cast<float>(rect.size.y) * job.scale.y// * 0.5f
        };

        // Obliczenie wspó³rzêdnych wierzcho³ków i tekstury
        sf::Vertex v[4];
        //v[0].position = pos + sf::Vector2f(-halfSize.x, -halfSize.y);
        //v[1].position = pos + sf::Vector2f(halfSize.x, -halfSize.y);
        //v[2].position = pos + sf::Vector2f(halfSize.x, halfSize.y);
        //v[3].position = pos + sf::Vector2f(-halfSize.x, halfSize.y);

        v[0].position = pos + sf::Vector2f(0.f, 0.f);
        v[1].position = pos + sf::Vector2f(size.x,0.f);
        v[2].position = pos + sf::Vector2f(size.x, size.y);
        v[3].position = pos + sf::Vector2f(0.f, size.y);

        v[0].texCoords = sf::Vector2f(static_cast<float>(rect.position.x), static_cast<float>(rect.position.y));
        v[1].texCoords = sf::Vector2f(static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y));
        v[2].texCoords = sf::Vector2f(static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y + rect.size.y));
        v[3].texCoords = sf::Vector2f(static_cast<float>(rect.position.x), static_cast<float>(rect.position.y + rect.size.y));

        sf::Transform transform;
        transform.translate(pos);
        sf::Vector2f origin = size * 0.5f;
        transform.rotate(job.rotation, origin);

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

    if (C.debugMode) {
        for (auto [e, col, trans, render] : C.registry.view<Collider, Transform, Renderable>().each()) {
            const sf::Vector2f scaledSizeA = { col.size.x * trans.scale.x,col.size.y * trans.scale.y };
            const sf::Vector2f scaledOffsetA = { col.offset.x * trans.scale.x, col.offset.y * trans.scale.y };
            const sf::Vector2f globalPositionA = trans.position + scaledOffsetA;

            sf::RectangleShape shape;
            std::cout << trans.scale.x << ", " << trans.scale.y << "\n";
            shape.setSize(scaledSizeA);
            shape.setPosition(globalPositionA);
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineThickness(1);
            shape.setOutlineColor(sf::Color::Red);
            window.draw(shape);
            //  std::cout <<rect.getSize
        }
    }

    //C.qtree.draw(&window);
    //C.qtree.reset();
}