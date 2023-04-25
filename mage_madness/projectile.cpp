#include "projectile.h"
#include "cloud.h"
#include "maths.h"

using namespace sf;
using namespace std;


Projectile::Projectile() : Entity() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(16,16)));
	setTexture(whiteBallTex);
	setColor(Color::Cyan);

	_dest = {0,0};
	_isActive = false;
	_isBouncy = false;
	_hp = 0;
	_angleShot = 0;
	_angleShot2 = 0;

	_speed = 500;
};

void Projectile::Update(const float& dt) {

	if (getState()) {

		if (_isBouncy) {
			if (_hp == 2) {
				move({ (cos(_angleShot) * _speed) * dt, 0 });
				move({ 0, (sin(_angleShot) * _speed) * dt });
			}
			else {
				move({ (cos(_angleShot2) * _speed) * dt, 0 });
				move({ 0, (sin(_angleShot2) * _speed) * dt });
			}
		}
		else {
			move({ (cos(_angleShot) * _speed) * dt, 0 });
			move({ 0, (sin(_angleShot) * _speed) * dt });
		}
		
	}

	Entity::Update(dt);
}


void Projectile::fireMe(sf::Vector2f startPos, sf::Vector2f destination, int hp, float speed) {

	_isActive = true;
	_hp = hp;
	setPosition(startPos);
	_dest = destination;
	_speed = speed;

	_angleShot = atan2(destination.y - getPosition().y, destination.x - getPosition().x);
	_angleShot2 = -_angleShot;

	if (hp > 1) {
		_isBouncy = true;
	}
	else {
		_isBouncy = false;
	}
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
	setPosition({-10,-10 });
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

void Projectile::Render(sf::RenderWindow& window)
{
	Renderer::queue(this);
}


TeleProjectile::TeleProjectile() {
	
	_lifeSpan = 1;
	setColor(sf::Color::Magenta);
}

void TeleProjectile::Update(const float& dt) {

	if (_isActive) {

		if (_lifeSpan > 0) {

			_lifeSpan -= 1 * dt;
			//setScale(sf::Vector2f{ _lifeSpan , _lifeSpan });
		}
		else {

			resetProjectile();
			_lifeSpan = 1;
		}
	}

	Projectile::Update(dt);
}

void TeleProjectile::collision(const float& dt, sf::FloatRect collision, sf::FloatRect wall, std::shared_ptr<Player> player) {

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
	_lifeSpan = 1;
}

