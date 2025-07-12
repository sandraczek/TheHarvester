#include "MovementSystem.h"
#include <iostream>
#include "Event.hpp"

namespace WorldConstants {

	constexpr float GRAVITY_ACCELERATION = 2496.f;
	constexpr float MOVE_ACCELERATION = 3000.f;
	constexpr float JUMP_VELOCITY = -1024.f;
	constexpr float MAX_FALL_SPEED = 1600.f;
}

MovementSystem::MovementSystem(Context& C): C(C) {
    C.dispatcher.sink<CollisionEvent>().connect<&MovementSystem::onCollisionEvent>(*this);
}

void MovementSystem::update(float dTime) {
    for (auto [entity, transform, mov] : C.registry.view<Transform, Movement>().each()) {

        mov.acceleration = { 0.f, 0.f };

        if (auto* input = C.registry.try_get<InputState>(entity)) {
            mov.acceleration.x += input->moveDirection * WorldConstants::MOVE_ACCELERATION;
            if (auto* rend = C.registry.try_get<Renderable>(entity)) {
                if (input->moveDirection != 0.f)
                    rend->direction = input->moveDirection;
            }
        }

        if (auto* gravity = C.registry.try_get<Gravity>(entity)) {
            mov.acceleration.y += WorldConstants::GRAVITY_ACCELERATION;

            if (auto* input = C.registry.try_get<InputState>(entity)) {
                if (input->wantsToJump) {
                    if (gravity->onGround) {
                        mov.velocity.y = WorldConstants::JUMP_VELOCITY;
                    }
                    input->wantsToJump = false;
                }
            }
        }

        mov.velocity += mov.acceleration * dTime;

        bool onGround = C.registry.try_get<Gravity>(entity) ? C.registry.get<Gravity>(entity).onGround : false;
        float friction = onGround ? mov.groundFriction : mov.airFriction;

        if (C.registry.try_get<InputState>(entity) == nullptr || C.registry.get<InputState>(entity).moveDirection == 0.f) {
            mov.velocity.x *= std::pow(friction, dTime * 60.f);
        }

        if (std::abs(mov.velocity.x) < 1.f) {
            mov.velocity.x = 0.f;
        }

        mov.velocity.x = std::clamp(mov.velocity.x, -mov.maxSpeed, mov.maxSpeed);
        mov.velocity.y = std::clamp(mov.velocity.y, WorldConstants::JUMP_VELOCITY, WorldConstants::MAX_FALL_SPEED);

        transform.position += mov.velocity * dTime;

        if (auto* grav = C.registry.try_get<Gravity>(entity)) {
            grav->onGround = false;
        }
    }
}

void MovementSystem::onCollisionEvent(const CollisionEvent& event) {
    auto& trans1 = C.registry.get<Transform>(event.e1);
    auto& trans2 = C.registry.get<Transform>(event.e2);

    auto* mov = C.registry.try_get<Movement>(event.e1);

    if (C.registry.all_of<SolidTag>(event.e1)) {
        auto* grav = C.registry.try_get<Gravity>(event.e2);
        auto* mov = C.registry.try_get<Movement>(event.e2);
        if (event.intersection.size.x < event.intersection.size.y) {
            if (trans1.position.x > trans2.position.x)
                trans2.position.x -= event.intersection.size.x;
            else
                trans2.position.x += event.intersection.size.x;
            if (mov)
                mov->velocity.x = 0.f;
        }
        else {
            if (trans1.position.y > trans2.position.y) {
                trans2.position.y -= event.intersection.size.y;
                if (grav)
                    grav->onGround = true;
            }
            else
                trans2.position.y += event.intersection.size.y;
            if (mov)
                mov->velocity.y = 0.f;
        }
    }
    else if (C.registry.all_of<SolidTag>(event.e2)) {
        auto* grav = C.registry.try_get<Gravity>(event.e1);
        auto* mov = C.registry.try_get<Movement>(event.e1);
        if (event.intersection.size.x < event.intersection.size.y) {
            if (trans2.position.x > trans1.position.x)
                trans1.position.x -= event.intersection.size.x;
            else
                trans1.position.x += event.intersection.size.x;
            if (mov)
                mov->velocity.x = 0.f;
        }
        else {
            if (trans2.position.y > trans1.position.y) {
                trans1.position.y -= event.intersection.size.y;
                if (grav)
                    grav->onGround = true;
            }
            else
                trans1.position.y += event.intersection.size.y;
            if (mov)
                mov->velocity.y = 0.f;
        }
    }
    else {
        auto* mov1 = C.registry.try_get<Movement>(event.e1);
        auto* mov2 = C.registry.try_get<Movement>(event.e2);

        if (event.intersection.size.x < event.intersection.size.y) {
            if (trans1.position.x > trans2.position.x) {
                trans1.position.x += event.intersection.size.x * 0.5f;
                trans2.position.x -= event.intersection.size.x * 0.5f;
            }
            else {
                trans1.position.x += event.intersection.size.x * 0.5f;
                trans2.position.x -= event.intersection.size.x * 0.5f;
            }

            if (mov1)
                mov1->velocity.x = 0.f;
            if (mov2)
                mov2->velocity.x = 0.f;
        }
        else {
            if (trans1.position.y > trans2.position.y) {
                trans1.position.y += event.intersection.size.y * 0.5f;
                trans2.position.y -= event.intersection.size.y * 0.5f;
            }                                                
            else {                                           
                trans1.position.y += event.intersection.size.y * 0.5f;
                trans2.position.y -= event.intersection.size.y * 0.5f;
            }
            if (mov1)
                mov1->velocity.y = 0.f;
            if (mov2)
                mov2->velocity.y = 0.f;
        }
    }

}