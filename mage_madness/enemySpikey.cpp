#include "enemySpikey.h"
#include "cloud.h"
#include "player.h"
#include <iostream>

using namespace sf;
using namespace std;

// IDEAS -------------------------------------------------------------
//
// * if time permits we could detect when the spikey collides with tiles and make it semi transparent while moving through objects
// 
//--------------------------------------------------------------------

EnemySpikey::EnemySpikey(std::shared_ptr<Player> &player) : Entity()
{
	setTextureRect(IntRect(Vector2(0, 0), Vector2(46, 47)));
	setTexture(enemySpikeyTex);
	
	// Waypoints
	_waypoint1 = Vector2f(100, 100);
	_waypoint2 = Vector2f(100, 500);
	/*_waypoint3 = Vector2f(100, 100);
	_waypoint4 = Vector2f(100, 100);
	_waypoint5 = Vector2f(100, 100);*/

	_length = 0;
	_speed = 50;
	_timer = 10;
	_direction = Vector2f();
	_range = 50;
	_homePos = getPosition();
	_player = player;


	_currState = SpikeyState::Patrolling;


}

void EnemySpikey::Update(const float& dt)
{



	switch (_currState)
	{

		case SpikeyState::Patrolling:	// if the state is set patrolling, the entity will move side to side on the spot for a few seconds before travelling again
		{
			// Decrease the timer
			_timer -= 1 * dt;
			_movement = _direction * _speed * dt;
			move(_movement);

			// Right side
			if (_timer > 0 && getPosition().x > -100.f)
			{
				_direction = sf::Vector2f(-1.0f, 0.0f);
			}
			// Left side
			if (_timer > 0 && getPosition().x < 150.f)
			{
				_direction = sf::Vector2f(1.0f, 0.0f);
			}

			// Travel to next waypoint if time is 0
			if (_timer == 0)
			{
				_currState = SpikeyState::Travelling;
			}
		}
			break;
			
		case SpikeyState::Travelling:   // if the state is set travelling, the entity will choose a random point on the map and travel there, when at destination change to patrolling
			
			
				
				
				
			

			
			
			break;

		case SpikeyState::Following:	// if the state is set follow, the entity will follow the player (follow triggers when )
			
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

			break;

		default:
			break;
	}
	
	Entity::Update(dt);
}

void EnemySpikey::Render(sf::RenderWindow& window)
{
	Renderer::queue(this);
}