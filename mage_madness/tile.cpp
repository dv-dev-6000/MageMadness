#include "tile.h"
#include "cloud.h"

using namespace sf;
using namespace std;

Tile::Tile(std::string type, sf::Vector2f pos) : Sprite() {

	setTexture(tileTex);		// set to default tile **change to depend on type
	setTextureRect(IntRect(Vector2(0, 0), Vector2(64,64)));
	setPosition(pos);
};

void Tile::Update(const float& dt) {}

