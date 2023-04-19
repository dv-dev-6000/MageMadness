#include "enemyTurret.h"
#include "cloud.h"

using namespace sf;
using namespace std;

EnemyTurret::EnemyTurret() : Entity()
{
	setTextureRect(IntRect(Vector2(0, 0), Vector2(58, 64)));
	setTexture(enemyTurTex);
}

void EnemyTurret::Update(const float& dt)
{

	Entity::Update(dt);
}

void EnemyTurret::Render(sf::RenderWindow& window)
{
	Renderer::queue(this);
}