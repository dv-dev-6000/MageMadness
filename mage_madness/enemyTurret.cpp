#include "enemyTurret.h"
#include "player.h"
#include "projectile.h"
#include "cloud.h"

using namespace sf;
using namespace std;

EnemyTurret::EnemyTurret(std::shared_ptr<Player>& player, sf::Vector2f turPos) : Entity()
{
	setTextureRect(IntRect(Vector2(0, 0), Vector2(58, 64)));
	setTexture(enemyTurTex);

	_turPos = turPos;
	_player = player;
	_difference = {0,0};
	_distance = 0;
	_attackRange = 500;
	_timer = 3;
	_shoot = false;

	setPosition(turPos);
}

bool EnemyTurret::Shoot() {

	if (_shoot) {
		_shoot = false;
		return true;
	}
	else {
		return false;
	}
}

void EnemyTurret::Update(const float& dt)
{
	// reduce timer
	_timer -= 1 * dt;

	// if timer reaches zero
	if (_timer <= 0) {

		// Calculate the distance between the player and turret
		_difference = _player->getPosition() - getPosition();
		_distance = sqrt(pow(_difference.x, 2) + pow(_difference.y, 2));

		// if player is in turrets firing range
		if (_distance <= _attackRange)
		{
			_shoot = true;
		}

		_timer = 3;
	}

	Entity::Update(dt);
}

void EnemyTurret::Render(sf::RenderWindow& window)
{
	Renderer::queue(this);
}