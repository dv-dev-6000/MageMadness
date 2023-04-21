#include "enemySpikey.h"
#include "cloud.h"
#include "player.h"

using namespace sf;
using namespace std;

EnemySpikey::EnemySpikey(std::shared_ptr<Player> &player) : Entity()
{
	setTextureRect(IntRect(Vector2(0, 0), Vector2(46, 47)));
	setTexture(enemySpikeyTex);
	
	_length = 0;
	_speed = 50;

	_player = player;
}

void EnemySpikey::Update(const float& dt)
{
	// Calculate direction from enemy to player
	_direction = _player->getPosition() - getPosition();
	_length = sqrt(_direction.x * _direction.x + _direction.y * _direction.y);

	// If length not 0 then divide direction/length and move enemy towards player
	if (_length != 0.0f)
	{
		_direction /= _length;
	}
	// Move enemy spikey
	move(_direction * _speed * dt);

	Entity::Update(dt);
}

void EnemySpikey::Render(sf::RenderWindow& window)
{
	Renderer::queue(this);
}