#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class Button : public Entity {
protected:

    sf::Text _buttonText;
    bool _selected;
    int _id;

public:
    //Constructor
    Button(std::string text, int textSize, sf::Vector2f pos, int id);

    //Update
    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;
    int isSelected();

};