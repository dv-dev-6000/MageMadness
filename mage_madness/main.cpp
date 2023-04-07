#include <SFML/Graphics.hpp>
#include <iostream>
#include "cloud.h"
#include "LevelSystem.h"
#include "tile.h"
#include "player.h"
#include "projectile.h"

using namespace sf;
using namespace std;


// Game Variables =======================================================================================================

// list of game levels/scenes
enum class GameScene {

	mainMenu,
	tutorial_1,
	tutorial_2,
	tutorial_3,
	level_1,
	level_2,
	level_3,
	level_4,
	level_5
};

// list of game states
enum class GameState {

	playing,
	menu,
};

// var for current game scene
GameScene currScene;
GameState currState;

// get desktop resolution info
VideoMode desktop = VideoMode::getDesktopMode();
// create view object (used to scale content with window size and adjust aspect ratio) 
sf::View view;

// textures 
sf::Texture tileTex, breakTileTex, gravTileTex, spikeTileTex;
sf::Texture whiteBallTex;

std::vector<Tile> tiles;
std::vector<Projectile> projectiles;

sf::Texture playerTex;
Player player;

// Game Methods ===========================================================================================================

// manage window options
void ResetWindow(RenderWindow& window) {

	// disable repeated key events
	window.setKeyRepeatEnabled(false);
	// enable or disable vsync - do at start of game, or via options menu
	window.setVerticalSyncEnabled(true);
	// cap fps 
	window.setFramerateLimit(120);
	// set window view
	window.setView(view);
}


// Initialise ===========================================================================================================

/// <summary>
/// initialise variables
/// </summary>
void Init() {
	
	// set current game scene
	currScene = GameScene::mainMenu;
	currState = GameState::playing;
}

// Load Content =========================================================================================================

/// <summary>
/// Initial load - called on start up
/// </summary>
void Load() {

	// load tile textures
	if (!tileTex.loadFromFile("res/img/SpecialBlock3.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!breakTileTex.loadFromFile("res/img/SpecialBlock2.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!gravTileTex.loadFromFile("res/img/SpecialBlock.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!spikeTileTex.loadFromFile("res/img/SpikesBlue.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	// load player textures
	if (!playerTex.loadFromFile("res/img/PlayerSheet.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	// load projectile textures
	if (!whiteBallTex.loadFromFile("res/img/WhiteBall.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	//level load TEST
	ls::loadLevelFile("res/levels/maze.txt");

	for (int i = 0; i < ls::_sprites.size(); i++) {

		tileInfo currTile = ls::_sprites[i];

		Tile tile = Tile(currTile.type, currTile.pos);
		tiles.push_back(tile);
	}

	//load projectiles
	for (int i = 0; i < 10; i++) {

		Projectile p = Projectile();
		projectiles.push_back(p);
	}

	// load player 
	player.setTexture(playerTex);
	player.setPosition({ 100,100 });
}

/// <summary>
/// reload content called to transition between scenes
/// </summary>
void Reload() {

	// level logic
	switch (currScene) {

	case GameScene::mainMenu:

			// main menu logic here

			break;

		case GameScene::tutorial_1:

			// tut 1 logic here

			break;

		case GameScene::tutorial_2:

			// tut 2 logic here

			break;

		case GameScene::tutorial_3:

			// tut 3 logic here

			break;

		case GameScene::level_1:

			// level 1 logic here

			break;

		default:
			break;

	}


}


// Update Loop ==========================================================================================================

void Update(RenderWindow& window) {

	// Reset clock, recalculate deltatime
	static Clock clock;
	float dt = clock.restart().asSeconds();

	// check and consume events
	Event event;
	while (window.pollEvent(event)) {
		
		// key pressed events
		if (event.type == sf::Event::KeyPressed)
		{
			// player events if playing
			if (currState == GameState::playing) {

				// prep Jump
				if (event.key.code == Keyboard::Space) { 

					player.jumpPressed();
				}
				// flip player sprite
				if (event.key.code == Keyboard::D) {

					player.setTextureRect(IntRect(Vector2(0, 0), Vector2(45, 64)));
				}
				if (event.key.code == Keyboard::A) {

					player.setTextureRect(sf::IntRect(Vector2(45, 0), Vector2(-45,64)));
				}
			}


			if (event.key.code == Keyboard::Escape) {

				window.close();
			}
		}

		// key released events
		if (event.type == sf::Event::KeyReleased)
		{
			// player events if playing
			if (currState == GameState::playing) {

				if (event.key.code == Keyboard::Space) {

					player.jumpReleased();
				}
			}
			

			if (event.key.code == Keyboard::F1) {

				// minimaize and change aspect ratio -- move to options menu
				window.create(VideoMode({ 800,600 }, desktop.bitsPerPixel), "MageMadness", sf::Style::Default);
				view.setSize({ hdGameWidth, hdGameHeight + (hdGameHeight/3)});
				ResetWindow(window);
			}
			if (event.key.code == Keyboard::F2) {

				// full screen hd
				window.create(VideoMode(desktop.size, desktop.bitsPerPixel), "MageMadness", sf::Style::Fullscreen);
				view.setSize({ hdGameWidth, hdGameHeight });
				ResetWindow(window);
			}
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left) {

				// map mouse coords to world coords
				sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				projectiles[0].fireMe(player.getPosition(), mousePosition, 1);
			}
		}

		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}

	// update entities
	player.Update(dt);

	for (auto it = begin(projectiles); it != end(projectiles); ++it) {
		if (it->getState()) {
			it->Update(dt);
		}
	}

	// check collision with walls
	for (const auto s : tiles) {

		// get bounds for wall
		sf::FloatRect wBounds = s.getGlobalBounds();

		//check projectile collision
		for (auto it = begin(projectiles); it != end(projectiles); ++it) {

			// get projectile bounds
			sf::FloatRect projBounds = it->getGlobalBounds();
			// get collision data
			optional collision = wBounds.findIntersection(projBounds);

			if (it->getState() && collision) {
				
				// get collision rect
				FloatRect colRect = collision.value();

				it->collision(dt, colRect, wBounds, 1);
			}
		}


		//check player collision 
		sf::FloatRect pBounds = player.getGlobalBounds();
		optional collision = wBounds.findIntersection(pBounds);

		if (collision) {

			// get values for edges of collision rect [this rect represents the amount by which the player is intersecting with the tile]
			float colLeft = collision.value().left;
			float colRight = collision.value().left + collision.value().width;
			float colTop = collision.value().top;
			float colBottom = collision.value().top + collision.value().height;
			// get values for edges of wall rect
			float wallLeft = wBounds.left;
			float wallRight = wBounds.left + wBounds.width;
			float wallTop = wBounds.top;
			float wallBottom = wBounds.top + wBounds.height;

			if (colTop == wallTop && collision.value().width > collision.value().height) // players feet
			{
				player.resetVelocity(player.getVelX(), 0);
				player.resetJump();
				player.setPosition({ pBounds.left, wBounds.top - pBounds.height });
			}
			else if (colLeft == wallLeft && collision.value().width < collision.value().height) // players right
			{
				player.resetVelocity(0, player.getVelY());
				player.setPosition({ wBounds.left - pBounds.width, pBounds.top });
			}
			else  if (colRight == wallRight && collision.value().width < collision.value().height) // players left
			{
				player.resetVelocity(0, player.getVelY());
				player.setPosition({ wBounds.left + wBounds.width, pBounds.top });
			}
			else if (colBottom == wallBottom && collision.value().width > collision.value().height) // players head
			{
				player.resetVelocity(player.getVelX(), 0);
				player.setPosition({ pBounds.left, wBounds.top + wBounds.height + 5 });
			}
		}

		// old collision code (works but not ideal)
		// 
		//if (collision) {
		//
		//	if (pBounds.top < wBounds.top												// collision on players bottom
		//		&& pBounds.top + pBounds.height < wBounds.top + wBounds.height
		//		&& pBounds.left < wBounds.left + wBounds.width
		//		&& pBounds.left + pBounds.width > wBounds.left)
		//	{
		//		player.resetVelocity(player.getVelX(), 0);
		//		player.resetJump();
		//		player.setPosition({ pBounds.left, wBounds.top - pBounds.height });
		//	}
		//	else if (pBounds.left < wBounds.left										// collision on players right
		//		&& pBounds.left + pBounds.width < wBounds.left + wBounds.width
		//		&& pBounds.top < wBounds.top + wBounds.height
		//		&& pBounds.top + pBounds.height > wBounds.top)
		//	{
		//		player.resetVelocity(0, player.getVelY());
		//		player.setPosition({ wBounds.left - pBounds.width, pBounds.top });
		//	}
		//	else  if (pBounds.left > wBounds.left										// collision on players left
		//		&& pBounds.left + pBounds.width > wBounds.left + wBounds.width
		//		&& pBounds.top < wBounds.top + wBounds.height
		//		&& pBounds.top + pBounds.height > wBounds.top)
		//	{
		//		player.resetVelocity(0, player.getVelY());
		//		player.setPosition({ wBounds.left + wBounds.width, pBounds.top });
		//	}
		//	else if (pBounds.top > wBounds.top											// collision on players top
		//		&& pBounds.top + pBounds.height > wBounds.top + wBounds.height
		//		&& pBounds.left < wBounds.left + wBounds.width
		//		&& pBounds.left + pBounds.width > wBounds.left)
		//	{
		//		player.resetVelocity(player.getVelX(), 0);
		//		player.setPosition({ pBounds.left, wBounds.top + wBounds.height +5 });
		//	}
		//	
		//}
	}

}


// Draw =================================================================================================================

void Render(RenderWindow& window) {
	// Draw Everything

	for (const auto s : tiles) {
		window.draw(s);
	}
	for (auto s : projectiles) {
		if (s.getState()) {
			window.draw(s);
		}
	}

	window.draw(player);
}

int main() {
	
	// set up view
	view.setSize({ hdGameWidth, hdGameHeight });
	view.setCenter({ view.getSize().x / 2, view.getSize().y / 2 });
	// set initial window properties
	RenderWindow window(VideoMode(desktop.size, desktop.bitsPerPixel), "MageMadness", sf::Style::Fullscreen);
	ResetWindow(window);

	Init();
	Load();

	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;
}