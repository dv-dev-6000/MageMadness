#include "projectile.h"
#include "cloud.h"
#include "maths.h"

using namespace sf;
using namespace std;


Projectile::Projectile() : Sprite() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(16,16)));
	setTexture(whiteBallTex);

	_dest = {0,0};
	_isActive = false;
	_hp = 0;
	_angleShot = 0;
	_angleShot2 = 0;

	_speed = 500;
};

void Projectile::Update(const float& dt) {

	
	if (_hp == 2) {
		move({ (cos(_angleShot) * _speed) * dt, 0 });
		move({ 0, (sin(_angleShot) * _speed) * dt });
	}
	else {
		move({ (cos(_angleShot2) * _speed) * dt, 0 });
		move({ 0, (sin(_angleShot2) * _speed) * dt });
	}
	
}


void Projectile::fireMe(sf::Vector2f startPos, sf::Vector2f destination, int hp) {

	_isActive = true;
	_hp = 2;
	setColor(Color::Cyan);
	setPosition(startPos);
	_dest = destination;

	_angleShot = atan2(destination.y - getPosition().y, destination.x - getPosition().x);
	_angleShot2 = -_angleShot;
}

bool Projectile::getState() {
	return _isActive;
}

void Projectile::collision(const float& dt, sf::FloatRect collision, sf::FloatRect wall, int dmg) {

	_hp -= dmg;
	if (_hp <= 0) {
		_isActive = false;
		_dest = { 0,0 };
		_angleShot = 0;
		_angleShot2 = 0;
		_speed = 500;
		setPosition({0,0});
	}
	else {

		if ((collision.left == wall.left || collision.left+collision.width == wall.left+wall.width) && collision.width < collision.height) // hit on edge
		{
			// deflect
			_speed = -_speed;
		}
	}

}