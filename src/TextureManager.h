#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager{
    std::map<std::string,sf::Texture> textures;
public:
    sf::Texture& get(const std::string& name);
};
