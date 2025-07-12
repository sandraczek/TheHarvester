#pragma once
#include "Context.hpp"
#include <SFML/System.hpp>

class UserInputSystem
{
public:
	UserInputSystem(Context& C) : C(C){}
	void update();
private:
	Context& C;

    bool prevKeys[sf::Keyboard::KeyCount]{};
    bool currKeys[sf::Keyboard::KeyCount]{};

    void updateInputs() {
        for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
            prevKeys[i] = currKeys[i];
            currKeys[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
        }
    }

    bool isKeyPressed(sf::Keyboard::Key key) const {
        const int k = static_cast<int>(key);
        return currKeys[k] && !prevKeys[k];
    }

    bool isKeyReleased(sf::Keyboard::Key key) const {
        const int k = static_cast<int>(key);
        return !currKeys[k] && prevKeys[k];
    }

    bool isKeyHeld(sf::Keyboard::Key key) const {
        const int k = static_cast<int>(key);
        return currKeys[k];
    }
};

