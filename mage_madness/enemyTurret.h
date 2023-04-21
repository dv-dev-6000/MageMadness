#pragma once
#include "entity_management.h"
#include "player.h"
#include <SFML/Graphics.hpp>

class EnemyTurret : public Entity
{
protected:
    std::shared_ptr<Player> _player;
    sf::Vector2f _difference;
    float _distance;
    float _attackRange;
    float _timer;
    bool _shoot;

public:

    //Constructor
    EnemyTurret(std::shared_ptr<Player>& player);

    void Update(const float& dt) override;
    bool Shoot();
    void Render(sf::RenderWindow& window) override;

};