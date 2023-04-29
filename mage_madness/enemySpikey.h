#pragma once
#include "entity_management.h"
#include "player.h"
#include <SFML/Graphics.hpp>

enum class SpikeyState {
    Travelling,
    Patrolling,
    Following
};

class EnemySpikey : public Entity
{
protected:
    std::shared_ptr<Player> _player;
    sf::Vector2f _direction;
    sf::Vector2f _homePos;
    SpikeyState _currState;
    float _speed;
    float _length;
    float _timer;
    float _range;

public:

    //Constructor
    EnemySpikey(std::shared_ptr<Player> &player);

    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;

};