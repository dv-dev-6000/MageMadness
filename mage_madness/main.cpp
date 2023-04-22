#include <SFML/Graphics.hpp>
#include <iostream>
#include "cloud.h"
#include "LevelSystem.h"
#include "entity_management.h"
#include "tile.h"
#include "player.h"
#include "projectile.h"
#include "enemyTurret.h"
#include "enemySpikey.h"
#include "button.h"

using namespace sf;
using namespace std;

// Forward Declaration for functions ====================================================================================
void Reload();

// Game Variables =======================================================================================================

// list of game levels/scenes
enum class GameScene {

	// Menu
	mainMenu,
	// Tutorial levels
	tutorial_1,
	tutorial_2,
	tutorial_3,
	// Area 1 levels
	level_1,
	level_2,
	level_3,
	level_4,
	level_5,
	// Boss Area levels
	boss_level_1,
	boss_level_2,
	boss_level_3
};

// list of game states
enum class GameState {

	playing,
	menu,
};

// entities & lists  
EntityManager entityManager;
EntityManager menuButtonManager;

std::vector<shared_ptr<Tile>> tiles;
std::vector<shared_ptr<Projectile>> projectiles;
std::vector<shared_ptr<Button>> buttons;
std::shared_ptr<Player> player;
std::shared_ptr<EnemyTurret> enemyTurret;		// ** to do - if adding more turrets then they will need to be stored in a vector like projectiles
std::shared_ptr<EnemySpikey> enemySpikey;
std::shared_ptr<TeleProjectile> tp;

// var for current game scene
GameScene currScene;
GameState currState;

// menu scenes
bool optionMenuOpen;
bool howToPlayOpen;

// get desktop resolution info
VideoMode desktop = VideoMode::getDesktopMode();
// create view object (used to scale content with window size and adjust aspect ratio) 
sf::View view;

// Pixel Font
Font pixFont;
Text titleText;

// textures 
sf::Texture tileTex, breakTileTex, gravTileTex, spikeTileTex, bossBlockTileTex, area1BlockTileTex, endBlockTileTex;
sf::Texture optionsBackdrop, howToBackdrop;
sf::Texture whiteBallTex;
sf::Texture playerTex;
sf::Texture buttonTex;
sf::Texture enemyTurTex;
sf::Texture enemySpikeyTex;

sf::Sprite menuBackdropSprite;

// Player/enemies position values
Vector2f playerPosition(100, 100);
Vector2f enemTurPosition(1250, 350);
Vector2f enemSpikPosition(250,350);


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
	optionMenuOpen = false;
	howToPlayOpen = false;
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
	if (!area1BlockTileTex.loadFromFile("res/img/area1Block.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!bossBlockTileTex.loadFromFile("res/img/bossLeveBlock.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!endBlockTileTex.loadFromFile("res/img/endBlock.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	if (!optionsBackdrop.loadFromFile("res/img/optionsMM.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!howToBackdrop.loadFromFile("res/img/howtoMM.png")) {
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
	// load projectile textures
	if (!buttonTex.loadFromFile("res/img/Button.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	// Loading enemies
	if (!enemyTurTex.loadFromFile("res/img/Turret.png"))
	{
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!enemySpikeyTex.loadFromFile("res/img/SpikeyFollow.png"))
	{
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	pixFont.loadFromFile("res/fonts/PressStart2P-Regular.ttf");
	titleText.setFont(pixFont);

	Reload();
}

/// <summary>
/// reload content called to transition between scenes
/// </summary>
void Reload() {

	// clear lists--------------------------------------------------------
	tiles.clear();
	projectiles.clear();
	entityManager.list.clear();
	menuButtonManager.list.clear();
	buttons.clear();
	titleText.setString(" ");

	// re-populate lists -------------------------------------------------

	// load player + add player to em list
	player = std::make_shared<Player>();
	entityManager.list.push_back(player);
	// load projectiles 
	for (int i = 0; i < 10; i++) {

		std::shared_ptr<Projectile> p = std::make_shared<Projectile>();
		projectiles.push_back(p);
		entityManager.list.push_back(p);
	}
	// load single tele projectile + add tp to em list
	tp = std::make_shared<TeleProjectile>();
	entityManager.list.push_back(tp);

	// Load turret enemy
	enemyTurret = make_shared<EnemyTurret>(player);
	// Load enemy spikey
	enemySpikey = make_shared<EnemySpikey>(player);

	// level logic -------------------------------------------------------
	switch (currScene) {

		case GameScene::mainMenu:

			// main menu logic here
			titleText.setCharacterSize(100);
			titleText.setString("Mage Madness");
			titleText.setPosition({ (view.getSize().x * .5f) - (titleText.getLocalBounds().width * .5f), 150 });
			
			if (!optionMenuOpen && !howToPlayOpen) {
				
				// load buttons main menu
				std::shared_ptr<Button> but1 = make_shared<Button>("New Game", 40, sf::Vector2f((view.getCenter().x - 224), 400), 1);
				menuButtonManager.list.push_back(but1);
				buttons.push_back(but1);

				std::shared_ptr<Button> but2 = make_shared<Button>("Continue", 40, sf::Vector2f((view.getCenter().x - 224), 550), 2);
				menuButtonManager.list.push_back(but2);
				buttons.push_back(but2);

				std::shared_ptr<Button> but3 = make_shared<Button>("How To Play", 35, sf::Vector2f((view.getCenter().x - 224), 700), 3);
				menuButtonManager.list.push_back(but3);
				buttons.push_back(but3);

				std::shared_ptr<Button> but4 = make_shared<Button>("Options", 40, sf::Vector2f((view.getCenter().x - 224), 850), 4);
				menuButtonManager.list.push_back(but4);
				buttons.push_back(but4);
			}
			else if (optionMenuOpen) {
				// add backdrop image
				menuBackdropSprite.setTexture(optionsBackdrop);
				// load buttons option menu
				std::shared_ptr<Button> but5 = make_shared<Button>("Do it!", 35, sf::Vector2f(100, 400), 5);
				menuButtonManager.list.push_back(but5);
				buttons.push_back(but5);

				std::shared_ptr<Button> but6 = make_shared<Button>("Do it!", 35, sf::Vector2f(100, 630), 6);
				menuButtonManager.list.push_back(but6);
				buttons.push_back(but6);

				std::shared_ptr<Button> but7 = make_shared<Button>("Do it!", 35, sf::Vector2f(100, 865), 7);
				menuButtonManager.list.push_back(but7);
				buttons.push_back(but7);

				std::shared_ptr<Button> but8 = make_shared<Button>("Do it!", 35, sf::Vector2f((view.getCenter().x - 224), 400), 8);
				menuButtonManager.list.push_back(but8);
				buttons.push_back(but8);

				std::shared_ptr<Button> but9 = make_shared<Button>("Do it!", 35, sf::Vector2f((view.getCenter().x - 224), 750), 9);
				menuButtonManager.list.push_back(but9);
				buttons.push_back(but9);

				std::shared_ptr<Button> but10 = make_shared<Button>("Do it!", 35, sf::Vector2f(1380, 470), 10);
				menuButtonManager.list.push_back(but10);
				buttons.push_back(but10);

				std::shared_ptr<Button> but11 = make_shared<Button>("Back", 35, sf::Vector2f(1380,910), 11);
				menuButtonManager.list.push_back(but11);
				buttons.push_back(but11);
			}
			else if (howToPlayOpen) {
				// add backdrop image
				menuBackdropSprite.setTexture(howToBackdrop);
				// load buttons how to play
				std::shared_ptr<Button> but11 = make_shared<Button>("Back", 35, sf::Vector2f((view.getCenter().x - 224), 800), 11);
				menuButtonManager.list.push_back(but11);
				buttons.push_back(but11);
			}

			break;

		case GameScene::tutorial_1:

			// tut 1 logic here

			// level load
			ls::loadLevelFile("res/levels/tutOne.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ playerPosition });
			// Set enemy turret values
			enemyTurret->setTexture(enemyTurTex);
			enemyTurret->setPosition({enemTurPosition});
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({enemSpikPosition});

			break;

		case GameScene::tutorial_2:

			// tut 2 logic here

			// level load
			ls::loadLevelFile("res/levels/tutTwo.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ 100,100 });

			break;

		case GameScene::tutorial_3:

			// tut 3 logic here

			break;

		case GameScene::level_1:

			// level 1 logic here

			break;

		case GameScene::level_2:

			// level 2 logic here

			break;
		case GameScene::level_3:

			// level 3 logic here

			break;
		case GameScene::level_4:

			// level 4 logic here

			break;
		case GameScene::level_5:

			// level 5 logic here

			break;
		case GameScene::boss_level_1:

			// boss level 1 logic here

			break;
		case GameScene::boss_level_2:

			// boss level 2 logic here

			break;
		case GameScene::boss_level_3:

			// boss level 3 logic here

			break;
		default:
			break;
	}

	// add enemies
	entityManager.list.push_back(enemyTurret);
	entityManager.list.push_back(enemySpikey);
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

					player->jumpPressed();
				}
				// flip player sprite
				if (event.key.code == Keyboard::D) {

					player->setTextureRect(IntRect(Vector2(0, 0), Vector2(45, 64)));
				}
				if (event.key.code == Keyboard::A) {

					player->setTextureRect(sf::IntRect(Vector2(45, 0), Vector2(-45, 64)));
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

					player->jumpReleased();
				}

				// TESTING SCENE SKIP
				if (event.key.code == Keyboard::Right) {

					if (currScene == GameScene::mainMenu) {
						currScene = GameScene::tutorial_1;
					}
					else {
						currScene = GameScene::tutorial_2;
					}
					Reload();
				}
			}
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left) {

				// determine if ingame or main menu
				if (currScene != GameScene::mainMenu) {

					// if ingame shoot projectile
					
					// map mouse coords to world coords
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					projectiles[0]->fireMe(player->getPosition(), mousePosition, 2, 500);
				}
				else {

					//if in menu, find if button is clicked and get button id
					for (auto buts = begin(buttons); buts != end(buttons); ++buts) {

						int id = (*buts)->isSelected();
						if (id != 0) {
							
							switch(id)
							{
								// determine action based on pressed button id
								case 1:
									// start from level one
									currScene = GameScene::tutorial_1;
									Reload();
									break;
								case 2:
									// load level to start from
									break;
								case 3:
									// display how to play panel
									howToPlayOpen = true;
									Reload();
									break;
								case 4:
									// open options
									optionMenuOpen = true;
									Reload();
									break;
								case 5:
									// full screen hd
									window.create(VideoMode(desktop.size, desktop.bitsPerPixel), "MageMadness", sf::Style::Fullscreen);
									view.setSize({ hdGameWidth, hdGameHeight });
									ResetWindow(window);
									break;
								case 6:
									// windowed
									window.create(VideoMode({ 1280, 720 }), "MageMadness", sf::Style::Default);
									view.setSize({ hdGameWidth, hdGameHeight });
									ResetWindow(window);
									break;
								case 7:
									// windowed 4:3
									// minimaize and change aspect ratio -- move to options menu
									window.create(VideoMode({ 800,600 }, desktop.bitsPerPixel), "MageMadness", sf::Style::Default);
									view.setSize({ hdGameWidth, hdGameHeight + (hdGameHeight / 3) });
									ResetWindow(window);
									break;
								case 8:
									// right hand
									break;
								case 9:
									// left hand
									break;
								case 10:
									// gamepad
									break;
								case 11:
									// back
									optionMenuOpen = false;
									howToPlayOpen = false;
									Reload();
									break;
								default:
									break;
							}
							break;
						}
					}
				}
				
			}
			if (event.mouseButton.button == sf::Mouse::Right) {

				// map mouse coords to world coords
				sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				tp->fireMe(player->getPosition(), mousePosition, 1, 500);
			}
		}

		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}

	// update entities
	if (currScene != GameScene::mainMenu) {
		entityManager.update(dt);
	}
	else {
		menuButtonManager.update(dt);
	}

	// check collision with walls
	for (auto s = begin(tiles); s != end(tiles); ++s) {

		// get bounds for wall
		sf::FloatRect wBounds = (*s)->getGlobalBounds();

		//check projectile collision
		for (auto it = begin(projectiles); it != end(projectiles); ++it) {

			// get projectile bounds
			sf::FloatRect projBounds = (*it)->getGlobalBounds();
			// get collision data
			optional collision = wBounds.findIntersection(projBounds);

			if ((*it)->getState() && collision) {

				// if breakblock then move offscreen
				if ((*s)->getType() == 3) {
					(*s)->setPosition({ -128, -128 });
				}
				// get collision rect
				FloatRect colRect = collision.value();
				// trigger collision effect
				(*it)->collision(dt, colRect, wBounds, 1);
			}
		}

		// check gravblocks
		if ((*s)->getType() == 2) {
			// tmp collision bool
			bool isColliding = false;
			// collision rect 2px lower than current position, used to check for potential collision under block
			FloatRect nextPos = { {(*s)->getGlobalBounds().getPosition().x, (*s)->getGlobalBounds().getPosition().y + 2}, {(*s)->getGlobalBounds().getSize()} };
			// check current grav tile against all tiles in level
			for (auto s2 = begin(tiles); s2 != end(tiles); ++s2) {
				// exculde checking tile for collision with itself
				if (s != s2) {
					// if the current tiles next position will cause a collison, set tmp colliding to true
					if ((*s2)->getGlobalBounds().findIntersection(nextPos)) {
						isColliding = true;
						break;
					}
				}
			}
			(*s)->setColliding(isColliding);
		}


		// check tp projectile collision
		// get projectile bounds
		sf::FloatRect tpBounds = tp->getGlobalBounds();
		// get collision data
		optional tpCol = wBounds.findIntersection(tpBounds);

		if (tp->getState() && tpCol) {

			// get collision rect
			FloatRect colRect = tpCol.value();

			tp->collision(dt, colRect, wBounds, player);
		}


		//check player collision 
		sf::FloatRect pBounds = player->getGlobalBounds();
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

			// if spiked then kill else collide
			if ((*s)->getType() == 4) {
				player->setPosition({ 64,64 });
				player->resetVelocity(0, 0);
			}
			else {

				if (colTop == wallTop && collision.value().width > collision.value().height) // players feet
				{
					player->resetVelocity(player->getVelX(), 0);
					player->resetJump();
					player->setPosition({ pBounds.left, wBounds.top - pBounds.height });
				}
				else if (colLeft == wallLeft && collision.value().width < collision.value().height) // players right
				{
					player->resetVelocity(0, player->getVelY());
					player->setPosition({ wBounds.left - pBounds.width, pBounds.top });
				}
				else  if (colRight == wallRight && collision.value().width < collision.value().height) // players left
				{
					player->resetVelocity(0, player->getVelY());
					player->setPosition({ wBounds.left + wBounds.width, pBounds.top });
				}
				else if (colBottom == wallBottom && collision.value().width > collision.value().height) // players head
				{
					player->resetVelocity(player->getVelX(), 0);
					player->setPosition({ pBounds.left, wBounds.top + wBounds.height + 5 });
				}
			}
		}
	}

	// check if turrets can shoot		*** once multiple turrets supported this shoudld cycle through a list of all turrets
	if (enemyTurret->Shoot()) {

		std::shared_ptr<Projectile> p = std::make_shared<Projectile>();
		projectiles.push_back(p);
		entityManager.list.push_back(p);
		
		p->fireMe(enemyTurret->getPosition(), player->getPosition(), 1, 300);

	}

}


// Draw =================================================================================================================

void Render(RenderWindow& window) {
	// Draw Everything
	if (currScene == GameScene::mainMenu) {
		window.draw(titleText);
		if (optionMenuOpen || howToPlayOpen) { 
			window.draw(menuBackdropSprite);
		}
		menuButtonManager.render(window);
		Renderer::render();
	}
	else {
		entityManager.render(window);
		Renderer::render();
	}
}

int main() {
	

	// set up view
	view.setSize({ hdGameWidth, hdGameHeight });
	view.setCenter({ view.getSize().x / 2, view.getSize().y / 2 });
	// set initial window properties
	RenderWindow window(VideoMode(desktop.size, desktop.bitsPerPixel), "MageMadness", sf::Style::Fullscreen);
	ResetWindow(window);
	Renderer::initialise(window);

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