#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class EnemyTurret : public Entity
{
public:

    //Constructor
    EnemyTurret();

    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;

};