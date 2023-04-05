#include "player.h"
#include "cloud.h"

using namespace sf;
using namespace std;


Player::Player() : Sprite() {

	setTextureRect(IntRect(Vector2(0, 0), Vector2(64, 64)));
	setTexture(playerTex);
};

void Player::Update(const float& dt) {



}


