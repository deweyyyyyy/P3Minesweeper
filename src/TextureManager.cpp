#include "TextureManager.h"

sf::Texture& TextureManager::get(const std::string& name){
    auto it = textures.find(name);
    if(it != textures.end()) return it->second;

    sf::Texture tex;
    tex.loadFromFile("files/images/" + name);
    auto res = textures.emplace(name, std::move(tex));
    return res.first->second;
}
