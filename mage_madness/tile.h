#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class Tile : public Entity {
protected:

    int _type;
    bool _gBlockColliding;

public:
    //Constructor that takes a sprite
    Tile(int type, sf::Vector2f pos);
    //Update, virtual so can be overridden, but not pure virtual
    void Update(const float& dt) override;

    int getType();
    bool getColliding();
    void setColliding(bool value);
};