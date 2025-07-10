#include "MovementSystem.h"
#include "Component.hpp"

namespace WorldConstants {

	constexpr float GRAVITY_ACCELERATION = 2800.f;
	constexpr float MOVE_ACCELERATION = 3000.f;
	constexpr float JUMP_VELOCITY = -800.f;
	constexpr float MAX_FALL_SPEED = 1200.f;
}

void MovementSystem::update(Context& C, float dTime) {
    for (const auto& [entity, pos, mov] : C.components.view<Transform, Movement>()) {

        mov.acceleration = { 0.f, 0.f };

        if (auto* input = C.components.try_get<InputState>(entity)) {
            mov.acceleration.x += input->moveDirection * WorldConstants::MOVE_ACCELERATION;
        }

        if (auto* grav = C.components.try_get<Gravity>(entity)) {
            if (!grav->onGround) {
                mov.acceleration.y += WorldConstants::GRAVITY_ACCELERATION;
            }

            if (auto* input = C.components.try_get<InputState>(entity)) {
                if (input->wantsToJump && grav->onGround) {
                    mov.velocity.y = WorldConstants::JUMP_VELOCITY;
                    grav->onGround = false;
                    input->wantsToJump = false;
                }
            }
        }

        mov.velocity += mov.acceleration * dTime;

        bool onGround = C.components.try_get<Gravity>(entity) ? C.components.get<Gravity>(entity).onGround : false;
        float friction = onGround ? mov.groundFriction : mov.airFriction;

        if (C.components.try_get<InputState>(entity) == nullptr || C.components.get<InputState>(entity).moveDirection == 0.f) {
            mov.velocity.x *= std::pow(friction, dTime * 60.f);
        }

        if (std::abs(mov.velocity.x) < 1.f) {
            mov.velocity.x = 0.f;
        }

        mov.velocity.x = std::clamp(mov.velocity.x, -mov.maxSpeed, mov.maxSpeed);
        mov.velocity.y = std::clamp(mov.velocity.y, -WorldConstants::JUMP_VELOCITY, WorldConstants::MAX_FALL_SPEED);

        pos.position += mov.velocity * dTime;
    }
}