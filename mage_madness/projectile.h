#pragma once
#include <SFML/Graphics.hpp>

class Projectile : public sf::Sprite {
protected:

    bool _isActive;
    int _hp;
    sf::Vector2f _dest;

    float _angleShot;
    float _angleShot2;
    float _speed;

public:

    //Constructor that takes a sprite
    Projectile();
    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& dt);

    void fireMe(sf::Vector2f startPos, sf::Vector2f destination, int hp);
    bool getState();
};