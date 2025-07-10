#pragma once
#include <cstdint>
#include <unordered_set>

using Entity = uint32_t;
const Entity MAX_ENTITIES = 5000;

class EntityManager {
private:
    Entity nextId = 0;
    std::unordered_set<Entity> aliveEntities;
public:
    Entity create() {
        Entity e = nextId++ % MAX_ENTITIES;
        aliveEntities.insert(e);
        return e;
    }

    void destroy(Entity e) {
        aliveEntities.erase(e);
    }

    const std::unordered_set<Entity>& getAll() const {
        return aliveEntities;
    }
};