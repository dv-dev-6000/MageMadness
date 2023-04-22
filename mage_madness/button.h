#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class Button : public Entity {
protected:

    sf::Text _buttonText;

public:
    //Constructor that takes a sprite
    Button(std::string text, int textSize);

    //Update
    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;

};