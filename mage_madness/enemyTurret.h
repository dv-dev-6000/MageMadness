#pragma once
#include "entity_management.h"
#include "player.h"
#include <SFML/Graphics.hpp>

class EnemyTurret : public Entity
{
protected:
    std::shared_ptr<Player> _player;
    sf::Vector2f _difference;
    sf::Vector2f _turPos;
    float _distance;
    float _attackRange;
    float _timer;
    bool _shoot;

public:

    //Constructor
    EnemyTurret(std::shared_ptr<Player>& player, sf::Vector2f turPos);

    void Update(const float& dt) override;
    bool Shoot();
    void Render(sf::RenderWindow& window) override;

};