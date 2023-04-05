#include <SFML/Graphics.hpp>
#include <iostream>
#include "cloud.h"
#include "LevelSystem.h"
#include "tile.h"
#include "player.h"

using namespace sf;
using namespace std;


// Game Variables =======================================================================================================

// list of game levels/scenes
enum GameScene {

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

// var for current game scene
GameScene currScene;

// get desktop resolution info
VideoMode desktop = VideoMode::getDesktopMode();
// create view object (used to scale content with window size and adjust aspect ratio) 
sf::View view;

// textures 
sf::Texture background;
sf::Texture back2;
sf::Texture tileTex, breakTileTex, gravTileTex, spikeTileTex;
//sprites
sf::Sprite bkSprite;
sf::Sprite bk2Sprite;

std::vector<Tile> tiles;

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
	window.setFramerateLimit(60);
	// set window view
	window.setView(view);
}


// Initialise ===========================================================================================================

/// <summary>
/// initialise variables
/// </summary>
void Init() {

	// set current game scene
	currScene = mainMenu;
	
}

// Load Content =========================================================================================================

/// <summary>
/// Initial load - called on start up
/// </summary>
void Load() {

	// load test images
	if (!background.loadFromFile("res/img/testBackground.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!back2.loadFromFile("res/img/TEST.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
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
	if (!playerTex.loadFromFile("res/img/Player.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	// set test image as sprite texture
	//bkSprite.setTexture(background);
	//bk2Sprite.setTexture(back2);
	// set values for source rect
	//bk2Sprite.setTextureRect(IntRect(Vector2(0, 0), Vector2(1920, 1080)));
	//bk2Sprite.setPosition({ 0,0 });
	//bkSprite.setTextureRect(IntRect(Vector2(0, 0), Vector2(800, 800)));
	//bkSprite.setPosition({ 0,0 });

	//level load TEST
	ls::loadLevelFile("res/levels/maze.txt");

	for (int i = 0; i < ls::_sprites.size(); i++) {

		tileInfo currTile = ls::_sprites[i];

		Tile tile = Tile(currTile.type, currTile.pos);
		tiles.push_back(tile);
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

		case mainMenu:

			// main menu logic here

			break;

		case tutorial_1:

			// tut 1 logic here

			break;

		case tutorial_2:

			// tut 2 logic here

			break;

		case tutorial_3:

			// tut 3 logic here

			break;

		case level_1:

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
		
		if (event.type == sf::Event::KeyReleased)
		{
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

		if (event.type == sf::Event::KeyPressed)
		{
			//// if S key pressed
			//if (event.key.code == Keyboard::S) {
			//
			//	bkSprite.setPosition(bkSprite.getPosition() + Vector2f(1, 1));
			//}
			// if ESC key pressed
			if (event.key.code == Keyboard::Escape) {

				window.close();
			}
		}

		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}

	// move when key held
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
	
		if (player.getPosition().y < (1080 - 64)) {
	
			player.setPosition(player.getPosition() + Vector2f(1, 1));
		}
		
	}

	// check player collision with walls
	for (const auto s : tiles) {
		sf::FloatRect pBounds = player.getGlobalBounds();
		sf::FloatRect wBounds = s.getGlobalBounds();

		if (wBounds.findIntersection(pBounds)) {
			player.setPosition({ 100,100 });
		}
	}

}



// Draw =================================================================================================================

void Render(RenderWindow& window) {
	// Draw Everything
	
	//window.draw(bk2Sprite);
	//window.draw(bkSprite);

	for (const auto s : tiles) {
		window.draw(s);
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