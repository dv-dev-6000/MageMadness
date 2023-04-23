#include "button.h"
#include "cloud.h"

using namespace sf;
using namespace std;


Button::Button(std::string text, int textSize, sf::Vector2f pos, int id) : Entity() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(448, 128)));
	setTexture(buttonTex);
	
	setPosition(pos);
	
	_selected = false;
	_id = id;

	_buttonText.setFont(pixFont);
	_buttonText.setCharacterSize(textSize);
	_buttonText.setString(text);
	_buttonText.setPosition({ (getPosition().x + 224) - (_buttonText.getLocalBounds().width * .5f), (getPosition().y + 64) - (_buttonText.getLocalBounds().height * .5f) });
};

void Button::Update(const float& dt) {

	if (getGlobalBounds().contains(cursor.getPosition())) {
		setColor(sf::Color::Green);
		_selected = true;
	}
	else {
		setColor(sf::Color::White);
		_selected = false;
	}

	Entity::Update(dt);
}

int Button::isSelected() {

	if (_selected) {
		_selected = false;
		return _id;
	}
	else { return 0; }
}

void Button::Render(sf::RenderWindow& window) {

	window.draw(_buttonText);
	Renderer::queue(this);
}