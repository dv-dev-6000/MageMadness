#pragma once
#include "entity_management.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <random>

enum class SpikeyState {
    Travelling,
    Patrolling,
    Following
};

class EnemySpikey : public Entity
{
protected:
    std::shared_ptr<Player> _player;
    float _speed;
    float _length;
    float _l;
    float _distW;
    sf::Vector2f _direction;
    sf::Vector2f _distance;
    SpikeyState _currState;
    sf::Vector2f _randomPos;
    sf::Vector2f _patrolPos;
    float _range;

    bool _isActive;

public:

    //Constructor
    EnemySpikey(std::shared_ptr<Player> &player);

    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;

    sf::Vector2f generateNewPoint();

    bool getActive();
    void setActive(bool value);

};