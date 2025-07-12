#include "Factory.h"

Factory::Factory(Context& C): C(C){}

entt::entity Factory::Player(const sf::Vector2f& pos) {
    auto e = C.registry.create();

    C.registry.emplace<Transform>(e, pos, sf::Vector2f{ 3.2f,3.2f });
    C.registry.emplace<Movement>(e);
    C.registry.emplace<Gravity>(e);
    C.registry.emplace<Health>(e, 100.f, 100.f);
    C.registry.emplace<Damage>(e, 10.f, 10.f);
    C.registry.emplace<PlayerTag>(e);
    C.registry.emplace<UserInput>(e);
    C.registry.emplace<InputState>(e);

    C.registry.emplace<Renderable>(e, "player", sf::IntRect{ {0,0}, { 46, 50 } }, 0);

    C.registry.emplace<Collider>(e, sf::Vector2f{ 46.f, 50.f });

    return e;
}
entt::entity Factory::Spider(const sf::Vector2f& pos) {
    auto e = C.registry.create();

    C.registry.emplace<Transform>(e, pos);
    C.registry.emplace<Movement>(e);
    C.registry.emplace<Gravity>(e);
    C.registry.emplace<AI>(e);
    C.registry.emplace<InputState>(e);
    C.registry.emplace<Health>(e, 100.f, 100.f);
    C.registry.emplace<Damage>(e, 10.f, 10.f);
    C.registry.emplace<EnemyTag>(e);

    C.registry.emplace<Renderable>(e, "critters", sf::IntRect{ {17,16}, { 30, 20 } }, 100);

    C.registry.emplace<Collider>(e, sf::Vector2f{ 30.f, 20.f });

    return e;
}








entt::entity Factory::Box(const sf::FloatRect& rect) {
    auto e = C.registry.create();

    const sf::IntRect textureRect({ 0,0 }, { 4, 4 });
    const sf::Vector2f scale = { rect.size.x / static_cast<float>(textureRect.size.x), rect.size.y / static_cast<float>(textureRect.size.y) };

    C.registry.emplace<Transform>(e, rect.position, scale);
    C.registry.emplace<SolidTag>(e);

    C.registry.emplace<Renderable>(e, "floor", textureRect, -10, false);

    C.registry.emplace<Collider>(e, static_cast<sf::Vector2f>(textureRect.size));

    return e;
}