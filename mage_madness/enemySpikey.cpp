#include "enemySpikey.h"
#include <random>
#include "cloud.h"
#include "player.h"
#include <iostream>

using namespace sf;
using namespace std;

EnemySpikey::EnemySpikey(std::shared_ptr<Player> &player) : Entity()
{
	setTextureRect(IntRect(Vector2(0, 0), Vector2(46, 47)));
	setTexture(enemySpikeyTex);
	_isActive = true;

	_distW = 0;
	_length = 0;
	_l = 0;
	_speed = 50;
	_player = player;
	_range = 250;
	_currState = SpikeyState::Travelling; // starting state - change to patrolling later

	_randomPos = generateNewPoint();
	_patrolPos = { _randomPos.x + 80, _randomPos.y };

}

sf::Vector2f EnemySpikey::generateNewPoint() {
	// Generate random numbers
	random_device _rd;
	mt19937 _gen(_rd());
	// Produce random x and y position accodring to screen width/height
	uniform_real_distribution<float> xAxis(64, 1820);
	uniform_real_distribution<float> yAxis(64, 880);

	// Random position
	sf::Vector2f _randomPos(xAxis(_gen), yAxis(_gen));

	return _randomPos;
}

void EnemySpikey::Update(const float& dt)
{

	if (_isActive) {

		switch (_currState){
			
			case SpikeyState::Patrolling:	// if the state is set patrolling, the entity will move side to side on the spot for a few seconds before travelling again

				// Calculate direction from waypoint to player
				_direction = _patrolPos - getPosition();
				_distW = sqrt(_direction.x * _direction.x + _direction.y * _direction.y);

				// If distance to waypoint is above 0.1 travel to it. 
				// Therefore, travel there
				if (_distW > 5.0f)
				{
					_direction /= _distW;
					move(_direction * _speed * dt);
				}
				else
				{
					if (_patrolPos == _randomPos) {
						_randomPos = generateNewPoint();
						_patrolPos = { _randomPos.x + 80, _randomPos.y };
						_currState = SpikeyState::Travelling;
						setColor(sf::Color::White);
					}
					else if (_patrolPos.x > _randomPos.x) {
						_patrolPos = { _randomPos.x - 80, _randomPos.y };
					}
					else {
						_patrolPos = _randomPos;
					}
				}

				_distance = _player->getPosition() - getPosition();
				_l = sqrt(_distance.x * _distance.x + _distance.y * _distance.y);
				// If player is in close range, change current state to following
				if (_l <= _range)
				{
					_currState = SpikeyState::Following;
					setColor(sf::Color::Cyan);
				}

				break;

			case SpikeyState::Travelling:   // if the state is set travelling, the entity will choose a random point on the map and travel there, when at destination change to patrolling
		
				// Calculate direction from waypoint to player
				_direction = _randomPos - getPosition();
				_distW = sqrt(_direction.x * _direction.x + _direction.y * _direction.y);

				// If distance to waypoint is above 0.1 travel to it. 
				// If distance is less than 0.1 then create next random waypoint
				// Therefore, travel there
				if (_distW > 5.0f)
				{
					_direction /= _distW;
					move(_direction * _speed * dt);
				}
				else
				{
					// if target reached go to patrolling
					_currState = SpikeyState::Patrolling;
					setColor(sf::Color::Red);
					//_randomPos = generateNewPoint();
				}


				_distance = _player->getPosition() - getPosition();
				_l = sqrt(_distance.x * _distance.x + _distance.y * _distance.y);
				// If player is in close range, change current state to following
				if (_l <= _range)
				{
					_currState = SpikeyState::Following;
					setColor(sf::Color::Cyan);
				}
		
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

	}

	Entity::Update(dt);
}

bool EnemySpikey::getActive() {
	return _isActive;
}

void EnemySpikey::setActive(bool value) {
	_isActive = value;
}

void EnemySpikey::Render(sf::RenderWindow& window)
{
	if (_isActive) {
		Renderer::queue(this);
	}
}