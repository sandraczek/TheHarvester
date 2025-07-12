#include "AnimationSystem.h"

void AnimationSystem::update(float dTime) {
    for (auto [entity, anim, renderable] : C.registry.view<AnimationState, Renderable>().each()) {
        anim.timer += dTime;
        const auto& animData = anim.animations.at(anim.currentState);

        if (anim.timer >= animData.duration) {
            anim.timer -= animData.duration;
            anim.currentFrame = (anim.currentFrame + 1) % animData.frameCount;

            int frameX = (animData.startFrame + anim.currentFrame) * renderable.frameSize.x;
            int frameY = 0; // Zak³adaj¹c, ¿e klatki s¹ w jednym rzêdzie
            renderable.textureRect = { { frameX, frameY }, renderable.frameSize };
        }
    }
    for (auto [entity, input, trans] : C.registry.view<InputState, Transform>().each()) {
       if (input.moveDirection > 0.f)
           if (trans.scale.x < 0)
               trans.scale.x = -trans.scale.x;
       else if (input.moveDirection < 0.f)
           if (trans.scale.x > 0)
                trans.scale.x = -trans.scale.x;
    }
}