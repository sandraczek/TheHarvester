#pragma once
#include <SFML/Graphics.hpp>

entt::entity spawnPlayer(Context& C, const sf::Vector2f& pos) {
    auto e = C.registry.create();
    auto& vel = C.registry.emplace<Transform>(e, pos, sf::Vector2f{2.f,2.f}, 0.f );
    C.registry.emplace<Movement>(e);
    C.registry.emplace<Gravity>(e);
    C.registry.emplace<Health>(e, 100.f, 100.f );
    C.registry.emplace<Damage>(e, 10.f,10.f );
    C.registry.emplace<PlayerTag>(e);

    C.registry.emplace<Renderable>(e, "player", sf::IntRect{{0,0}, { 46, 50 }}, 0);

    C.registry.emplace<Collider>(e, sf::FloatRect{ {0.f,0.f}, {44.f, 90.f} }); ////////

    return e;
}
entt::entity spawnSpider(Context& C, sf::Vector2f pos) {
    auto e = C.registry.create();
    C.registry.emplace<Transform>(e, pos);
    C.registry.emplace<Movement>(e);
    C.registry.emplace<Gravity>(e);
    C.registry.emplace<AI>(e);
    C.registry.emplace<Health>(e, 100.f, 100.f );
    C.registry.emplace<Damage>(e, 10.f,10.f );
    C.registry.emplace<EnemyTag>(e);
            
    C.registry.emplace<Renderable>(e, "critters", sf::IntRect{ {17,16}, { 30, 20 } }, 100);

    C.registry.emplace<Collider>(e, sf::FloatRect{ {0.f,0.f}, {30.f, 20.f} });

    return e;
}

void destroyEntity(Context& C, entt::entity e) {
    C.registry.destroy(e);
}