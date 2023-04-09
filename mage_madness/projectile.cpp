#include "projectile.h"
#include "cloud.h"
#include "maths.h"

using namespace sf;
using namespace std;


Projectile::Projectile() : Entity() {

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

void  Projectile::resetProjectile() {
	_isActive = false;
	_dest = { 0,0 };
	_angleShot = 0;
	_angleShot2 = 0;
	_speed = 500;
	setPosition({ 0,0 });
}

void Projectile::collision(const float& dt, sf::FloatRect collision, sf::FloatRect wall, int dmg) {

	_hp -= dmg;
	if (_hp <= 0) {
		resetProjectile();
	}
	else {

		if ((collision.left == wall.left || collision.left+collision.width == wall.left+wall.width) && collision.width < collision.height) // hit on edge
		{
			// deflect
			_speed = -_speed;
		}
	}

}

void TeleProjectile::collision(const float& dt, sf::FloatRect collision, sf::FloatRect wall, Player *player) {

	sf::Vector2f newpos = { 0,0 };

	if (collision.left == wall.left && collision.width < collision.height) // hit on walls left edge
	{
		newpos = { collision.left - player->getTextureRect().width, wall.top };
	}
	else if (collision.left + collision.width == wall.left + wall.width && collision.width < collision.height) // hit on walls right edge
	{
		newpos = { collision.left + collision.width, wall.top };
	}
	else if (collision.top == wall.top && collision.width > collision.height) // hit on walls top edge
	{
		newpos = { wall.left, collision.top - player->getTextureRect().height };
	}
	else if (collision.top + collision.height == wall.top + wall.height && collision.width > collision.height) // hit on walls bottom edge
	{
		newpos = { wall.left, collision.top + collision.height };
	}

	player->setPosition(newpos);
	player->resetVelocity(0, 0);

	_hp = 0;
	resetProjectile();
}