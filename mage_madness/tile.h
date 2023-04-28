#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class Tile : public Entity {
protected:

    int _type;
    bool _gBlockColliding;
    bool _endBlockActive;

public:
    //Constructor that takes a sprite
    Tile(int type, sf::Vector2f pos);
    //Update
    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;

    int getType();
    bool getColliding();
    void setColliding(bool value);
    bool getEndActive();
    void setEndActive(bool value);
};