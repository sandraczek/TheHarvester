#include "MovementSystem.h"
#include <iostream>

namespace WorldConstants {

	constexpr float GRAVITY_ACCELERATION = 2800.f;
	constexpr float MOVE_ACCELERATION = 3000.f;
	constexpr float JUMP_VELOCITY = -800.f;
	constexpr float MAX_FALL_SPEED = 1200.f;
}

void MovementSystem::update(Context& C, float dTime) {
    for (const auto& [entity, transform, mov] : C.registry.view<Transform, Movement>().each()) {

        mov.acceleration = { 0.f, 0.f };

        if (auto* input = C.registry.try_get<InputState>(entity)) {
            mov.acceleration.x += input->moveDirection * WorldConstants::MOVE_ACCELERATION;
        }

        if (auto* gravity = C.registry.try_get<Gravity>(entity)) {
            if (!gravity->onGround) {
                mov.acceleration.y += WorldConstants::GRAVITY_ACCELERATION;
            }

            if (auto* input = C.registry.try_get<InputState>(entity)) {
                if (input->wantsToJump && gravity->onGround) {
                    mov.velocity.y = WorldConstants::JUMP_VELOCITY;
                    gravity->onGround = false;
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
    }
}