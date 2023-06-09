#include "player.h"
#include "cloud.h"

using namespace sf;
using namespace std;

const Keyboard::Key keyControls[3] = {
    Keyboard::A,		    // Player left
    Keyboard::D,		    // Player right
    Keyboard::Space,		// Player jump
};

const Keyboard::Key keyControlsLefty[3] = {
    Keyboard::Left,		    // Player left
    Keyboard::Right,		// Player right
    Keyboard::RControl,		// Player jump
};

Player::Player() : Entity() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(45, 64)));
	setTexture(playerTex);

    _speed = 200;
    _velocityX = 0;
    _velocityY = 0;
    _gravity = 750;

    _isAirborn = false;
    _jumpPressed = false;
    _spellPressed = false;

    _jChargeTime = 0;
    _jChargeUnit = 1000;
    _sChargeTime = 0;
    _sChargeUnit = 1400;
    
    _movingLeft = false;
    _movingRight = false;

    _yPosOld = 0;
};

void Player::moveLeft() {

    _velocityX = -_speed;
    // flip sprite
    setTextureRect(sf::IntRect(Vector2(45, 0), Vector2(-45, 64)));
}

void Player::moveRight() {

    _velocityX = _speed;

    setTextureRect(IntRect(Vector2(0, 0), Vector2(45, 64)));
}

void Player::setMoving(bool left, bool right) {

    _movingLeft = left;
    _movingRight = right;
}

void Player::Update(const float& dt) {

    _yPosOld = getPosition().y;

    //left
    if ((Keyboard::isKeyPressed(keyControls[0]) && conScheme == 1) || (Keyboard::isKeyPressed(keyControlsLefty[0]) && conScheme == 2) || _movingLeft){
        
        moveLeft();
    }
    //right
    else if ((Keyboard::isKeyPressed(keyControls[1]) && conScheme == 1) || (Keyboard::isKeyPressed(keyControlsLefty[1]) && conScheme == 2) || _movingRight) {
        
        moveRight();
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

    if (_spellPressed) {
        if (_sChargeTime < _maxSpellCharge) {

            _sChargeTime += _sChargeUnit * dt;
        }
    }

    _velocityY += _gravity * dt;

    // check if falling
    if (_yPosOld != (int)getPosition().y) {
        _isAirborn = true;
    }
    _yPosOld = getPosition().y;

    Entity::Update(dt);
}

void Player::projectilePressed() {

    _spellPressed = true;
}

void Player::projectileReleased() {

    _spellPressed = false;
    _sChargeTime = 0;
}

void Player::jumpPressed() {

    if (!_isAirborn) {
        setTextureRect(IntRect(Vector2(45, 0), Vector2(45, 64)));
        _jumpPressed = true;
    }
    
}

void Player::jumpReleased() {

    if (!_isAirborn && _jumpPressed) {

        // play sound 
        sound.setBuffer(jumpSound);
        sound.play();

        _isAirborn = true;
        _jumpPressed = false;

        // set miniumu jump height
        if (_jChargeTime < 300) {
            // if charge to low use minimum height
            _velocityY = -300;
        }
        else {
            // else use charged value
            _velocityY = -_jChargeTime;
        }
        
        _jChargeTime = 0;
        setTextureRect(IntRect(Vector2(0, 0), Vector2(45, 64)));
    }
}

float Player::getJcharge() {
    return _jChargeTime;
}

float Player::getScharge() {
    return _sChargeTime;
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