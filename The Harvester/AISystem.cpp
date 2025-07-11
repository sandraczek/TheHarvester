#include "AISystem.h"
#include "IdleState.hpp"
#include "baseState.hpp"

void AISystem::update(Context& C, float dTime) {
    for (const auto& [entity,ai] : C.registry.view<AI>().each()) {
        if (!ai.currentState) {
            ai.currentState = std::make_shared<IdleState>();
            ai.currentState->enter(C, entity);
        }

        std::shared_ptr<AIState> nextState = ai.currentState->update(C, entity, dTime);

        if (nextState) {
            ai.currentState->exit(C, entity); 
            ai.currentState = nextState;      
            ai.currentState->enter(C, entity);
        }
    }
}