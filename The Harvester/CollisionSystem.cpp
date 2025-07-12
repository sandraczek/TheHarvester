#include "CollisionSystem.h"
#include <iostream>

void CollisionSystem::update() {
    auto view = C.registry.view<Collider, Transform>();

    for (auto [e1, col1, trans1] : view.each()) {
        const sf::Vector2f scaledSizeA = { col1.size.x * trans1.scale.x,col1.size.y * trans1.scale.y };
        const sf::Vector2f scaledOffsetA = { col1.offset.x * trans1.scale.x, col1.offset.y * trans1.scale.y };
        const sf::Vector2f globalPositionA = trans1.position + scaledOffsetA;
        const sf::FloatRect globalHitboxA(globalPositionA, scaledSizeA);


        for (auto [e2, col2, trans2] : view.each()) {
            if (e1 <= e2) continue;
            if (C.registry.all_of<SolidTag>(e1) && C.registry.all_of<SolidTag>(e2)) continue;

            const sf::Vector2f scaledSizeB = { col2.size.x * trans2.scale.x,col2.size.y * trans2.scale.y };
            const sf::Vector2f scaledOffsetB = { col2.offset.x * trans2.scale.x, col2.offset.y * trans2.scale.y };
            const sf::Vector2f globalPositionB = trans2.position + scaledOffsetB;
            const sf::FloatRect globalHitboxB(globalPositionB, scaledSizeB);

            if (auto intersection = globalHitboxA.findIntersection(globalHitboxB)) {
                C.dispatcher.trigger<CollisionEvent>(CollisionEvent{ e1, e2, *intersection });
            }
        }
    }
}
