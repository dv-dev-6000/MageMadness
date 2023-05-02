#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class PickUp: public Entity {
protected:

    bool _isActive;

public:
    //Constructor
    PickUp(sf::Vector2f pos);

    //Update
    void Update(const float& dt) override;
    //Render
    void Render(sf::RenderWindow& window) override;

    // methods
    void setActive(bool val);
    bool getActiveState();

};