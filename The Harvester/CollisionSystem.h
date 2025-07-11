#pragma once
#include <vector>
#include <set>
#include <utility>
#include "Context.hpp"
#include "Event.hpp"

class CollisionSystem {
public:
    CollisionSystem() = default;

    // G��wna funkcja wywo�ywana w ka�dej klatce
    void update(Context& C);

    // Zwraca list� zdarze� kolizji z ostatniej klatki
    const std::vector<CollisionEvent>& getCollisionEvents() const;

private:
    // Prywatne metody realizuj�ce poszczeg�lne fazy
    void updateColliderPositions(Context& C);
    void findAndStoreCollisionPairs(Context& C);
    void resolveCollisions(Context& C);

    // Funkcja pomocnicza do obliczania wektora separacji (Minimum Translation Vector)
    sf::Vector2f calculateMTV(const sf::FloatRect& a, const sf::FloatRect& b);

    // Zestaw unikalnych par encji, kt�re potencjalnie koliduj�
    std::set<std::pair<entt::entity, entt::entity>> m_collisionPairs;

    // Wektor zdarze� kolizji do publicznego wgl�du
    std::vector<CollisionEvent> m_collisionEvents;
};