#include "pickup.h"
#include "cloud.h"

using namespace sf;
using namespace std;


PickUp::PickUp(sf::Vector2f pos) : Entity() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(58,58)));
	setTexture(pickupTex);
	setPosition(pos);

}

void PickUp::Update(const float& dt) {


	Entity::Update(dt);
}

void PickUp::Render(sf::RenderWindow& window) {

	if (_isActive) {
		Renderer::queue(this);
	}
}

void PickUp::setActive(bool val) {
	_isActive = val;
}