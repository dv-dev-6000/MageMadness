#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class Player : public Entity {
protected:

    float _speed;
    float _velocityX;
    float _velocityY;
    float _gravity;

    const float _maxJumpCharge = 1000;
    float _jChargeTime;
    float _jChargeUnit;

    const float _maxSpellCharge = 1000;
    float _sChargeTime;
    float _sChargeUnit;

    bool _isAirborn;
    bool _jumpPressed;
    bool _spellPressed;
    bool _movingLeft;
    bool _movingRight;

    void moveLeft();
    void moveRight();

public:

    //Constructor
    Player();

    //Update, virtual so can be overridden, but not pure virtual
    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;

    void resetJump();
    void resetVelocity(float x, float y);
    void jumpPressed();
    void jumpReleased();
    void setMoving(bool left, bool right);

    void projectilePressed();
    void projectileReleased();

    float getVelX();
    float getVelY();
    float getJcharge();
    float getScharge();
};