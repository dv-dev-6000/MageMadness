#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class EnemySpikey : public Entity
{
protected:
    float _speed;

public:

    //Constructor
    EnemySpikey();

    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;

};