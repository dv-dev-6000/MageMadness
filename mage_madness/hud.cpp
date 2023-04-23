#include "hud.h"
#include "cloud.h"

using namespace sf;
using namespace std;


HUD::HUD() : Entity() {

	_failCount = 0;
	_gotCollectable = false;

	setTextureRect(IntRect(Vector2(0, 0), Vector2( 1920, 120)));
	setTexture(hudBaseTex);
	setPosition({ 0,960 });

	_failCountAsText = std::to_string(_failCount);

	_failText.setFont(pixFont);
	_failText.setCharacterSize(30);
	_failText.setString(_failCountAsText);
	_failText.setPosition({ 1190 - (_failText.getLocalBounds().width * .5f), 1020 });
}

void HUD::Update(const float& dt) {


	Entity::Update(dt);
}

void HUD::AddFail() {
	_failCount++;
	_failCountAsText = std::to_string(_failCount);
	_failText.setString(_failCountAsText);
}

void HUD::CollectableGained() {
	_gotCollectable = true;
	setTexture(hudOverTex);
}

void HUD::Render(sf::RenderWindow& window) {

	window.draw(_failText);
	Renderer::queue(this);
}