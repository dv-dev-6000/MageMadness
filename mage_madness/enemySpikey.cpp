#include "enemySpikey.h"
#include "cloud.h"

using namespace sf;
using namespace std;

EnemySpikey::EnemySpikey() : Entity()
{
	setTextureRect(IntRect(Vector2(0, 0), Vector2(46, 47)));
	setTexture(enemySpikeyTex);
}

void EnemySpikey::Update(const float& dt)
{

	Entity::Update(dt);
}

void EnemySpikey::Render(sf::RenderWindow& window)
{
	Renderer::queue(this);
}