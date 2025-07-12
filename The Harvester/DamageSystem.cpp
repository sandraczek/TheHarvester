#include "DamageSystem.h"

DamageSystem::DamageSystem(Context& C) : C(C) {
    C.dispatcher.sink<CollisionEvent>().connect<&DamageSystem::onCollisionEvent>(*this);
}

void DamageSystem::update(){}

void DamageSystem::onCollisionEvent(const CollisionEvent& event) {
    if (C.registry.all_of<Health>(event.e1) && C.registry.all_of<Damage>(event.e2)) {
        // entityB zada³o obra¿enia entityA
        auto& healthA = C.registry.get<Health>(event.e1);
        const auto& damageB = C.registry.get<Damage>(event.e2);
        healthA.current -= damageB.damage;
        // Tutaj mo¿esz te¿ wywo³aæ inne zdarzenie, np. EntityDamagedEvent
    }
    if (C.registry.all_of<Health>(event.e2) && C.registry.all_of<Damage>(event.e1)) {
        // entityA zada³o obra¿enia entityB
        auto& healthB = C.registry.get<Health>(event.e2);
        const auto& damageA = C.registry.get<Damage>(event.e1);
        healthB.current -= damageA.damage;
    }
}