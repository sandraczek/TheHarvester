#include "AnimationSystem.h"

void AnimationSystem::update(Context& C, float dTime) {
    for (const auto& [entity, anim, renderable] : C.registry.view<AnimationState, Renderable>().each()) {
        anim.timer += dTime;
        const auto& animData = anim.animations.at(anim.currentState);

        if (anim.timer >= animData.duration) {
            anim.timer -= animData.duration;
            anim.currentFrame = (anim.currentFrame + 1) % animData.frameCount;

            int frameX = (animData.startFrame + anim.currentFrame) * renderable.frameSize.x;
            int frameY = 0; // Zak�adaj�c, �e klatki s� w jednym rz�dzie
            renderable.textureRect = { { frameX, frameY }, renderable.frameSize };
        }
    }
}