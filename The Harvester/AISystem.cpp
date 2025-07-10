#include "AISystem.h"
#include "IdleState.hpp"

void AISystem::update(Context& C, float dTime) {
    for (auto& [entity,ai] : C.components.getAll<AI>()) {
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