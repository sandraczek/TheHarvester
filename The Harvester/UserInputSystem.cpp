#include "UserInputSystem.h"

void UserInputSystem::update() {

	updateInputs();

	for (auto [e, state] : C.registry.view<InputState, UserInput>().each()) {
		if (isKeyHeld(sf::Keyboard::Key::A) == isKeyHeld(sf::Keyboard::Key::D))
			state.moveDirection = 0.f;
		if (isKeyHeld(sf::Keyboard::Key::A))
			state.moveDirection = -1.f;
		if (isKeyHeld(sf::Keyboard::Key::D))
			state.moveDirection = 1.f;
		if (isKeyPressed(sf::Keyboard::Key::Space))
			state.wantsToJump = true;
	}
}