#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Resources {
private:
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    std::unordered_map<std::string, sf::Font> fonts;
public:
    void loadTexture(const std::string& name, const std::string& filepath) {
        sf::Texture tex;
        if (tex.loadFromFile(filepath)) {
            textures[name] = tex;
        }
    }

    const sf::Texture& getTexture(const std::string& name) {
        if (textures.find(name) == textures.end()) {
            throw std::runtime_error("Texture not found: " + name);
        }
        return textures.at(name);
    }
    void loadFont(const std::string& name, const std::string& filepath) {
        sf::Font font;
        if (font.openFromFile(filepath)) {
            fonts[name] = font;
        }
    }

    const sf::Font& getFont(const std::string& name) {
        if (fonts.find(name) == fonts.end()) {
            throw std::runtime_error("Font not found: " + name);
        }
        return fonts.at(name);
    }
};
