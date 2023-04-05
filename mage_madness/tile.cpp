#include "tile.h"
#include "cloud.h"

using namespace sf;
using namespace std;


Tile::Tile(int type, sf::Vector2f pos) : Sprite() {

			// set to default tile **change to depend on type
	
	switch (type) {

		case 1:
			// if wall
			setTexture(tileTex);
			break;

		case 2:
			// if grav block
			setTexture(gravTileTex);
			break;

		case 3:
			// if break block
			setTexture(breakTileTex);
			break;

		case 4:
			// if spike
			setTexture(spikeTileTex);
			break;

		case 5:
			// if end
			setTexture(tileTex); // UPDATE TEXTURE
			break;
		default:
			break;
	}

	setTextureRect(IntRect(Vector2(0, 0), Vector2(64,64)));
	setPosition(pos);
};

void Tile::Update(const float& dt) {}


