#include "enemySpikey.h"
#include <random>
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
	
	_length = 0;
	_speed = 50;
	_range = 20;
	_player = player;

	_currState = SpikeyState::Travelling; // starting state - change to patrolling later
}

void EnemySpikey::Update(const float& dt)
{

	switch (_currState)
	{
		case SpikeyState::Patrolling:	// if the state is set patrolling, the entity will move side to side on the spot for a few seconds before travelling again

			// decrease timer by 1*dt																		** to do

			// set curr position as "homePosition"															** to do
			// set target location 30px left of homePosition												** to do

			// if timer is > 0 then move towards target location											** to do
			// else timer is zero change to travelling then reset timer										** to do

			// if entity reaches target destination move target destination 30px right of home position		** to do

			break;
			
		case SpikeyState::Travelling:   // if the state is set travelling, the entity will choose a random point on the map and travel there, when at destination change to patrolling
		{
			// Generate random numbers
			random_device _rd;
			mt19937 _gen(_rd());
			// Produce random x and y position accodring to screen width/height
			uniform_real_distribution<float> xAxis(30, 1900);
			uniform_real_distribution<float> yAxis(30, 1050);

			// Random position
			Vector2f _randomPos(xAxis(_gen), yAxis(_gen));

			// Calculate direction from waypoint to player
			_direction = _randomPos - getPosition();
			_length = sqrt(_direction.x * _direction.x + _direction.y * _direction.y);

			// If distance to waypoint is above 0.1 travel to it. 
			// If distance is less than 0.1 then create next random waypoint
			// Therefore, travel there
			if (_length > 0.1f)
			{
				_direction /= _length;
				move(_direction * _speed * dt);
			}
			else
			{
				_randomPos = Vector2f(xAxis(_gen), yAxis(_gen));
			}

			// If player is in close range, change current state to following
			if (_length <= _range)
			{
				_currState = SpikeyState::Following;
				setColor(sf::Color::Red);
			}
		}
			break;

		case SpikeyState::Following:	// if the state is set follow, the entity will follow the player (follow triggers when )
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
		}
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