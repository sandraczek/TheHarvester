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
            renderable.texture,
            renderable.direction
        };

        renderQueue.push_back(j);
    }

    std::sort(renderQueue.begin(), renderQueue.end());

    window.clear(backgroundColor);


    for (auto& job : renderQueue) {
        sf::Sprite sprite(*job.texture);
        sprite.setTextureRect(job.texRect);
        sprite.setPosition(job.position);
        sprite.setRotation(job.rotation);

        if (job.direction < 0.f) {
            sprite.setOrigin({sprite.getLocalBounds().size.x, 0.f});
            sprite.setScale({ -job.scale.x, job.scale.y });
        }
        else
            sprite.setScale(job.scale);

        window.draw(sprite);
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