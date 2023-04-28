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

	_jumpRect.setPosition({ 309, 985 });
	_jumpRect.setSize({ 0,26 });
	_jumpRect.setFillColor(sf::Color::Cyan);

	_spellRect.setPosition({ 309, 1033 });
	_spellRect.setSize({ 0,26 });
	_spellRect.setFillColor(sf::Color::Magenta);
}

void HUD::Update(const float& dt) {

	_spellRect.setSize({ _spellBarX,26 });
	_jumpRect.setSize({ _jumpBarX,26 });

	Entity::Update(dt);
}

void HUD::AddFail() {
	_failCount++;
	_failCountAsText = std::to_string(_failCount);
	_failText.setString(_failCountAsText);
}

void HUD::SetFail(int val) {
	_failCount = val;
	_failCountAsText = std::to_string(_failCount);
	_failText.setString(_failCountAsText);
}

int HUD::getFail() {
	return _failCount;
}

void HUD::CollectableGained() {
	_gotCollectable = true;
	setTexture(hudOverTex);
}

void HUD::JumpX(float val) {
	
	if (val > 0) {
		_jumpBarX = val / 2;
	}
	else { _jumpBarX = 0; }
	
}

void HUD::SpellX(float val) {

	if (val > 0) {
		_spellBarX = val / 2;
	}
	else { _spellBarX = 0; }
}

void HUD::Render(sf::RenderWindow& window) {

	window.draw(_jumpRect);
	window.draw(_spellRect);
	window.draw(_failText);
	Renderer::queue(this);
}