#include "CollisionSystem.h"
#include <iostream>

void CollisionSystem::update(Context& C) {
    // Krok 0: Wyczyœæ stan z poprzedniej klatki
    m_collisionPairs.clear();
    m_collisionEvents.clear();

    // Krok 1: Zsynchronizuj pozycje koliderów z pozycjami encji
    updateColliderPositions(C);

    // Krok 2 (Broad Phase): U¿yj Quadtree, aby znaleŸæ potencjalne pary kolizji
    findAndStoreCollisionPairs(C);

    // Krok 3 i 4 (Narrow Phase & Resolution): SprawdŸ pary i rozwi¹¿ kolizje
    resolveCollisions(C);
}

const std::vector<CollisionEvent>& CollisionSystem::getCollisionEvents() const {
    return m_collisionEvents;
}

// --- Metody prywatne ---

void CollisionSystem::updateColliderPositions(Context& C) {
    for (const auto& [entity, transform, collider] : C.registry.view<Transform, Collider>().each()) {
        collider.bounds.position = transform.position + collider.offset;
    }
}

void CollisionSystem::findAndStoreCollisionPairs(Context& C) {

    for (auto [entity, collider] : C.registry.view<Collider>().each()) {
        for (auto [otherEntity, otherCollider] : C.registry.view<Collider>().each()) {
                if (entity >= otherEntity) continue;
                m_collisionPairs.insert({ entity, otherEntity });
        }
    }
}

void CollisionSystem::resolveCollisions(Context& C) {
    for (const auto& pair : m_collisionPairs) {
        entt::entity entityA = pair.first;
        entt::entity entityB = pair.second;

        auto* a_transform = C.registry.try_get<Transform>(entityA);
        auto* a_collider = C.registry.try_get<Collider>(entityA);
        auto* b_transform = C.registry.try_get<Transform>(entityB);
        auto* b_collider = C.registry.try_get<Collider>(entityB);

        // Upewnij siê, ¿e obie encje wci¹¿ maj¹ potrzebne komponenty
        if (!a_transform || !a_collider || !b_transform || !b_collider) {
            continue;
        }

        // Faza w¹ska (Narrow Phase) - dok³adne sprawdzenie
        sf::Vector2f mtv = calculateMTV( a_collider->bounds, b_collider->bounds);
        //std::cout << "\na: " << a_collider->bounds.position.y +a_collider->bounds.size.y;
        //std::cout << "\nb: " << b_collider->bounds.position.y +b_collider->bounds.size.y;
        std::cout << "\nc: " << b_collider->bounds.position.y;
        std::cout << "\nx: " << b_transform->position.y;
        // Jeœli wektor MTV ma d³ugoœæ 0, to nie ma kolizji
        if (mtv.x == 0.f && mtv.y == 0.f) {
            continue;
        }
        // JEST KOLIZJA!

        // Faza zdarzeñ (Event Phase)
        m_collisionEvents.push_back({ entityA, entityB });

        // Faza rozwi¹zywania (Resolution)
        if (C.registry.all_of<SolidTag>(entityA)) {
            // Jeœli A jest statyczne, B musi siê odsun¹æ
            b_transform->position -= mtv;
        }
        else if (C.registry.all_of<SolidTag>(entityB)) {
            // Jeœli B jest statyczne, A musi siê odsun¹æ
            a_transform->position += mtv;
        }
        else {
            // Jeœli oba s¹ dynamiczne, rozsuñ je po równo
            a_transform->position += mtv * 0.5f;
            b_transform->position -= mtv * 0.5f;
        }
    }
}

sf::Vector2f CollisionSystem::calculateMTV(const sf::FloatRect& a, const sf::FloatRect& b) {
    // Sprawdzenie osi X
    float overlapX = (a.position.x + a.size.x) - b.position.x;
    //std::cout << "1";
    if (overlapX <= 0) return {};
    float overlapX2 = (b.position.x + b.size.x) - a.position.x;
   // std::cout << "2";
    if (overlapX2 <= 0) return {};

    // Sprawdzenie osi Y
    float overlapY = (a.position.y + a.size.y) - b.position.y;
    //std::cout << "3";
    if (overlapY <= 0) return {};
    float overlapY2 = (b.position.y + b.size.y) - a.position.y;
   // std::cout << "4";
    if (overlapY2 <= 0) return {};

    // ZnajdŸ najmniejszy overlap
    float minOverlapX = std::min(overlapX, overlapX2);
    float minOverlapY = std::min(overlapY, overlapY2);

    // Wektor separacji (MTV) jest najmniejszym z overlapów
    if (minOverlapX < minOverlapY) {
        //std::cout << minOverlapX;
        return { a.position.x < b.position.x ? -minOverlapX : minOverlapX, 0.f };
    }
    else {
        //std::cout << minOverlapY;
        return { 0.f, a.position.y < b.position.y ? -minOverlapY : minOverlapY };
    }
}