#include "button.h"
#include "cloud.h"

using namespace sf;
using namespace std;


Button::Button(std::string text, int textSize) : Entity() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(320, 128)));
	setTexture(buttonTex);
	
	_buttonText.setFont(pixFont);
	_buttonText.setCharacterSize(textSize);
	_buttonText.setString(text);
	_buttonText.setPosition({ (getPosition().x + 160) - (_buttonText.getLocalBounds().width * .5f), (getPosition().y + 64) - (_buttonText.getLocalBounds().height * .5f) });
};

void Button::Update(const float& dt) {

	

	Entity::Update(dt);
}

void Button::Render(sf::RenderWindow& window) {

	window.draw(_buttonText);
	Renderer::queue(this);
}