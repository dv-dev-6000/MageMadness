#include "player.h"
#include "cloud.h"

using namespace sf;
using namespace std;

const Keyboard::Key keyControls[3] = {
    Keyboard::A,		    // Player left
    Keyboard::D,		    // Player right
    Keyboard::Space,		// Player jump
};

const Mouse::Button mouseControls[2] = {
    Mouse::Left,		    // M left
    Mouse::Right,		    // M right
};

Player::Player() : Entity() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(45, 64)));
	setTexture(playerTex);

    _speed = 200;
    _velocityX = 0;
    _velocityY = 0;
    _gravity = 9;

    _isAirborn = false;
    _jumpPressed = false;

    _jChargeTime = 0;
    _jChargeUnit = 700;
    
};

void Player::Update(const float& dt) {

    //left
    if (Keyboard::isKeyPressed(keyControls[0])) {
        
        _velocityX = -_speed;
    }
    //right
    else if (Keyboard::isKeyPressed(keyControls[1])) {
        
        _velocityX = _speed;
    }
    else {
        _velocityX = 0;
    }

    if (!_jumpPressed) {

        move({ _velocityX * dt, _velocityY * dt });
    }
    else {

        if (_jChargeTime < _maxJumpCharge) {

            _jChargeTime += _jChargeUnit*dt;
        }
    }


    _velocityY += _gravity;

    Entity::Update(dt);
}

void Player::projectilePressed() {

    
}

void Player::projectileReleased() {

    
}

void Player::jumpPressed() {

    if (!_isAirborn) {
        setTextureRect(IntRect(Vector2(45, 0), Vector2(45, 64)));
        _jumpPressed = true;
    }
    
}

void Player::jumpReleased() {

    if (!_isAirborn && _jumpPressed) {
        _isAirborn = true;
        _jumpPressed = false;
        _velocityY = -_jChargeTime;
        _jChargeTime = 0;
        setTextureRect(IntRect(Vector2(0, 0), Vector2(45, 64)));
    }
}

void Player::resetJump() {

    _isAirborn = false;
}

void Player::resetVelocity(float x, float y) {

    _velocityX = x;
    _velocityY = y;
}

float Player::getVelX() {
    return _velocityX;
}

float Player::getVelY() {
    return _velocityY;
}

void Player::Render(sf::RenderWindow& window)
{
    Renderer::queue(this);
}