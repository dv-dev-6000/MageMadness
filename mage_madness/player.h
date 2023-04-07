#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::Sprite {
protected:

    float _speed;
    float _velocityX;
    float _velocityY;
    float _gravity;

    const float _maxJumpCharge = 1000;
    float _jChargeTime;
    float _jChargeUnit;

    bool _isAirborn;
    bool _jumpPressed;

public:

    //Constructor
    Player();

    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& dt);

    void resetJump();
    void resetVelocity(float x, float y);
    void jumpPressed();
    void jumpReleased();

    void projectilePressed();
    void projectileReleased();

    float getVelX();
    float getVelY();
};