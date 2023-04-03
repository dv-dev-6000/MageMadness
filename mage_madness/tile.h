#pragma once
#include <SFML/Graphics.hpp>

class Tile : public sf::Sprite {
protected:

public:
    //Constructor that takes a sprite
    Tile(std::string type, sf::Vector2f pos);
    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& dt);
};