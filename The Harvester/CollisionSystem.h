#pragma once
#include <vector>
#include <set>
#include <utility>
#include "Context.hpp"
#include "Event.hpp"

class CollisionSystem {
public:
    CollisionSystem() = default;

    // G³ówna funkcja wywo³ywana w ka¿dej klatce
    void update(Context& C);

    // Zwraca listê zdarzeñ kolizji z ostatniej klatki
    const std::vector<CollisionEvent>& getCollisionEvents() const;

private:
    // Prywatne metody realizuj¹ce poszczególne fazy
    void updateColliderPositions(Context& C);
    void findAndStoreCollisionPairs(Context& C);
    void resolveCollisions(Context& C);

    // Funkcja pomocnicza do obliczania wektora separacji (Minimum Translation Vector)
    sf::Vector2f calculateMTV(const sf::FloatRect& a, const sf::FloatRect& b);

    // Zestaw unikalnych par encji, które potencjalnie koliduj¹
    std::set<std::pair<entt::entity, entt::entity>> m_collisionPairs;

    // Wektor zdarzeñ kolizji do publicznego wgl¹du
    std::vector<CollisionEvent> m_collisionEvents;
};