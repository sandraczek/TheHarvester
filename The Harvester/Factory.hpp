#pragma once
#include "Context.hpp"
#include "Component.hpp"
#include <SFML/Graphics.hpp>

/*
Entity spawnPlayer(Context& C, sf::Vector2f pos) {
    Entity e = C.entities.create();
    C.components.add<Transform>(e, { pos });
    C.components.add<Movement>(e, {});
    C.components.add<Gravity>(e, {});
    C.components.add<Health>(e, { 100.f, 100.f });
    C.components.add<Damage>(e, { 10.f,10.f });

    C.components.add<Sprite>(e, { &R.textures["player"], sf::IntRect{ { 0,0 }, { 46,50 } } });

    C.components.add<Collider>(e, { { {pos.x, pos.y}, {22.f, 45.f} } });
    return e;
}

Entity spawnSpider(Context& C, sf::Vector2f pos) {
    Entity e = C.entities.create();
    C.components.add<Transform>(e, { pos });
    C.components.add<Movement>(e, {});
    C.components.add<Gravity>(e, {});
    C.components.add<AI>(e, {});
    C.components.add<Health>(e, { 100.f, 100.f });
    C.components.add<Damage>(e, { 10.f,10.f });
    C.components.add<EnemyTag>(e, {});

    C.components.add<Renderable>(e, { "critters", sf::IntRect{ { 17,16 }, { 30,20 } } });

    C.components.add<Collider>(e, { { {pos.x, pos.y}, {30.f, 20.f} } });
    return e;
}
*/

Entity spawnPlayer(Context& C, sf::Vector2f pos) {
    Entity e = C.entities.create();
    C.components.add<Transform>(e, { pos });
    C.components.add<Movement>(e, {});
    C.components.add<Gravity>(e, {});
    C.components.add<Health>(e, { 100.f, 100.f });
    C.components.add<Damage>(e, { 10.f,10.f });
    C.components.add<PlayerTag>(e, {});

    C.components.add<Renderable>(e, Renderable{
        "player",
        {{0,0}, { 46, 50 }},
        100 // zIndex
    });

    C.components.add<Collider>(e, { { {pos.x, pos.y}, {22.f, 45.f} } });

    return e;
}
Entity spawnSpider(Context& C, sf::Vector2f pos) {
    Entity e = C.entities.create();
    C.components.add<Transform>(e, { pos });
    C.components.add<Movement>(e, {});
    C.components.add<Gravity>(e, {});
    C.components.add<AI>(e, {});
    C.components.add<Health>(e, { 100.f, 100.f });
    C.components.add<Damage>(e, { 10.f,10.f });
    C.components.add<EnemyTag>(e, {});

    C.components.add<Renderable>(e, Renderable{
        "critters",
        {{17,16}, { 30, 20 }},
        200 // zIndex
    });

    C.components.add<Collider>(e, { { {pos.x, pos.y}, {30.f, 20.f} } });

    return e;
}

void destroyEntity(Context& C, Entity e) {
    C.entities.destroy(e);
    C.components.removeAll(e);
}