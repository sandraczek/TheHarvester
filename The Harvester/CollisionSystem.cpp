#include "CollisionSystem.h"
#include <iostream>

void CollisionSystem::update(Context& C) {
    // Krok 0: Wyczy�� stan z poprzedniej klatki
    m_collisionPairs.clear();
    m_collisionEvents.clear();

    // Krok 1: Zsynchronizuj pozycje kolider�w z pozycjami encji
    updateColliderPositions(C);

    // Krok 2 (Broad Phase): U�yj Quadtree, aby znale�� potencjalne pary kolizji
    findAndStoreCollisionPairs(C);

    // Krok 3 i 4 (Narrow Phase & Resolution): Sprawd� pary i rozwi�� kolizje
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

        // Upewnij si�, �e obie encje wci�� maj� potrzebne komponenty
        if (!a_transform || !a_collider || !b_transform || !b_collider) {
            continue;
        }

        // Faza w�ska (Narrow Phase) - dok�adne sprawdzenie
        sf::Vector2f mtv = calculateMTV( a_collider->bounds, b_collider->bounds);
        //std::cout << "\na: " << a_collider->bounds.position.y +a_collider->bounds.size.y;
        //std::cout << "\nb: " << b_collider->bounds.position.y +b_collider->bounds.size.y;
        std::cout << "\nc: " << b_collider->bounds.position.y;
        std::cout << "\nx: " << b_transform->position.y;
        // Je�li wektor MTV ma d�ugo�� 0, to nie ma kolizji
        if (mtv.x == 0.f && mtv.y == 0.f) {
            continue;
        }
        // JEST KOLIZJA!

        // Faza zdarze� (Event Phase)
        m_collisionEvents.push_back({ entityA, entityB });

        // Faza rozwi�zywania (Resolution)
        if (C.registry.all_of<SolidTag>(entityA)) {
            // Je�li A jest statyczne, B musi si� odsun��
            b_transform->position -= mtv;
        }
        else if (C.registry.all_of<SolidTag>(entityB)) {
            // Je�li B jest statyczne, A musi si� odsun��
            a_transform->position += mtv;
        }
        else {
            // Je�li oba s� dynamiczne, rozsu� je po r�wno
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

    // Znajd� najmniejszy overlap
    float minOverlapX = std::min(overlapX, overlapX2);
    float minOverlapY = std::min(overlapY, overlapY2);

    // Wektor separacji (MTV) jest najmniejszym z overlap�w
    if (minOverlapX < minOverlapY) {
        //std::cout << minOverlapX;
        return { a.position.x < b.position.x ? -minOverlapX : minOverlapX, 0.f };
    }
    else {
        //std::cout << minOverlapY;
        return { 0.f, a.position.y < b.position.y ? -minOverlapY : minOverlapY };
    }
}