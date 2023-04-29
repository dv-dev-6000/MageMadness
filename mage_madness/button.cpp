#include "button.h"
#include "cloud.h"

using namespace sf;
using namespace std;


Button::Button(std::string text, int textSize, sf::Vector2f pos, int id) : Entity() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(448, 128)));
	setTexture(buttonTex);
	
	setPosition(pos);
	
	_isActive = true;
	_selected = false;
	_id = id;

	_buttonText.setFont(pixFont);
	_buttonText.setCharacterSize(textSize);
	_buttonText.setString(text);
	_buttonText.setPosition({ (getPosition().x + 224) - (_buttonText.getLocalBounds().width * .5f), (getPosition().y + 64) - (_buttonText.getLocalBounds().height * .5f) });
	if (text == "Exit") {
		_buttonText.setFillColor(sf::Color(255, 102, 102, 255));
	}
};

void Button::Update(const float& dt) {

	if (_isActive) {

		if (getGlobalBounds().contains(cursor.getPosition())) {
			setColor(sf::Color::Green);
			_selected = true;
		}
		else {
			if (_buttonText.getString() == "Exit") {
				setColor(sf::Color(255, 102, 102, 255));
			}
			else {
				setColor(sf::Color::White);
			}
			_selected = false;
		}
	}
	else {
		setColor(sf::Color(211, 211, 211, 180));
		_buttonText.setFillColor(sf::Color(211, 211, 211, 180));
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

void Button::setActive(bool val) {
	_isActive = val;
}

void Button::Render(sf::RenderWindow& window) {

	window.draw(_buttonText);
	Renderer::queue(this);
}