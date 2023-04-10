#pragma once
#include <SFML/Graphics.hpp>
#include "entity_management.h"
#include "player.h"

class Projectile : public Entity {
protected:

    bool _isActive;
    int _hp;
    sf::Vector2f _dest;

    float _angleShot;
    float _angleShot2;
    float _speed;

    void resetProjectile();

public:

    //Constructor
    Projectile();
    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& dt) override;

    void fireMe(sf::Vector2f startPos, sf::Vector2f destination, int hp);
    bool getState();
    virtual void collision(const float& dt, sf::FloatRect collision, sf::FloatRect wall, int dmg);
};

class TeleProjectile : public Projectile {
protected:

public:


    void collision(const float& dt, sf::FloatRect collision, sf::FloatRect wall, std::shared_ptr<Player> player);
};