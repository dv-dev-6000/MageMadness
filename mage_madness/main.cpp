#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
#include "hud.h"
#include "pickup.h"

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
//std::vector<shared_ptr<EnemyTurret>> turrets;
std::shared_ptr<EnemyTurret> t1;
std::shared_ptr<EnemyTurret> t2;
std::vector<shared_ptr<Button>> buttons;
std::shared_ptr<Player> player;
std::shared_ptr<PickUp> pickup;
std::shared_ptr<EnemySpikey> enemySpikey;
std::shared_ptr<TeleProjectile> tp;
std::shared_ptr<HUD> hud;

// var for current game scene
GameScene currScene;
GameState currState;
int conScheme; // 1=right, 2=left, 3=controler

// menu scenes
bool optionMenuOpen;
bool howToPlayOpen;

// cursor move with controller
int moveX;
int moveY;
const int cursorSpeed = 1000;

// get desktop resolution info
VideoMode desktop = VideoMode::getDesktopMode();
// create view object (used to scale content with window size and adjust aspect ratio) 
sf::View view;

// Pixel Font
Font pixFont;
Text titleText;

// textures 
sf::Texture tileTex, breakTileTex, gravTileTex, spikeTileTex, bossBlockTileTex, area1BlockTileTex, endBlockTileTex, upDownSpikesTex;
sf::Texture optionsBackdrop, howToBackdrop, howToBackdropLefty, howToBackdropController;
sf::Texture hudBaseTex, hudOverTex;
sf::Texture cursorTex;
sf::Texture whiteBallTex;
sf::Texture playerTex;
sf::Texture buttonTex;
sf::Texture enemyTurTex;
sf::Texture enemySpikeyTex;
sf::Texture pickupTex;

Vector2f initialPlayerPosition;

// backdrop sprite
sf::Sprite menuBackdropSprite;
// cursor sprite
sf::Sprite cursor;

// Player/enemies position values
Vector2f playerPosition(100, 100);
Vector2f enemSpikPosition(250,350);


// Sound
//sf::Music BGmusic;


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
	// disable cursor
	window.setMouseCursorVisible(false);
}

void PressButton(int id, RenderWindow& window) {

	switch (id)
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
			if (conScheme == 1) {
				menuBackdropSprite.setTexture(howToBackdrop);
			}
			else if (conScheme == 2) {
				menuBackdropSprite.setTexture(howToBackdropLefty);
			}
			else{
				menuBackdropSprite.setTexture(howToBackdropController);
			}
			howToPlayOpen = true;
			Reload();
			break;
		case 4:
			// open options
			menuBackdropSprite.setTexture(optionsBackdrop);
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
			conScheme = 1;
			menuBackdropSprite.setTexture(howToBackdrop);
			optionMenuOpen = false;
			howToPlayOpen = true;
			Reload();
			break;
		case 9:
			// left hand
			conScheme = 2;
			menuBackdropSprite.setTexture(howToBackdropLefty);
			optionMenuOpen = false;
			howToPlayOpen = true;
			Reload();
			break;
		case 10:
			// gamepad
			conScheme = 3;
			menuBackdropSprite.setTexture(howToBackdropController);
			optionMenuOpen = false;
			howToPlayOpen = true;
			Reload();
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

}

void MoveCursor(float dt) {

	cursor.move({moveX*dt, moveY*dt});
}

// Left click or controller right shoulder
void ClickOne(RenderWindow& window) {

	if (player->getScharge() < 995) {
		projectiles[0]->fireMe(player->getPosition(), cursor.getPosition(), 1, 500);
	}
	else {
		projectiles[0]->fireMe(player->getPosition(), cursor.getPosition(), 2, 500);
	}
	player->projectileReleased();
}

// right click or controller left shoulder
void ClickTwo(RenderWindow& window) {

	tp->fireMe(player->getPosition(), cursor.getPosition(), 1, 500);
}

void clickButton(RenderWindow& window) {
	
	//find if button is clicked and get button id
	for (auto buts = begin(buttons); buts != end(buttons); ++buts) {

		int id = (*buts)->isSelected();
		if (id != 0) {
			PressButton(id, window);
			break;
		}

	}
}

void KillPlayer() {
	player->setPosition({ initialPlayerPosition });
	player->resetVelocity(0, 0);
	hud->AddFail();
}

void TurretShoot(std::shared_ptr<EnemyTurret> turret) {

	if (turret->Shoot()) {

		// iterate through projectile pool, excluding element zero (reserved for player)
		for (auto it = 1; it < projectiles.size(); it++) {

			// find an inactive projectile in pool
			if (!projectiles[it]->getState()) {

				projectiles[it]->fireMe(turret->getPosition(), player->getPosition(), 1, 300);
				break;
			}
		}
	}

}

// Initialise ===========================================================================================================

/// <summary>
/// initialise variables
/// </summary>
void Init() {
	
	// set current game scene
	currScene = GameScene::mainMenu;
	currState = GameState::playing;
	conScheme = 1;
	optionMenuOpen = false;
	howToPlayOpen = false;
	moveX = false;
	moveY = false;
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
	if (!upDownSpikesTex.loadFromFile("res/img/SpikesUpDown.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	if (!optionsBackdrop.loadFromFile("res/img/optionsMM.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!howToBackdrop.loadFromFile("res/img/howtoMM.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!howToBackdropLefty.loadFromFile("res/img/howtoMM_Lefty.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!howToBackdropController.loadFromFile("res/img/howtoMM_Controller.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	if (!cursorTex.loadFromFile("res/img/Cursor.png")) {
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
	// load button textures
	if (!buttonTex.loadFromFile("res/img/Button.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	// load collectable textures
	if (!pickupTex.loadFromFile("res/img/diamond.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	// load hudbase
	if (!hudBaseTex.loadFromFile("res/img/HUDbase.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	// load hudover
	if (!hudOverTex.loadFromFile("res/img/HUDover.png")) {
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


	// Load sound
	/*if (!BGmusic.openFromFile("res/audio/BG.wav"))
	{
		cerr << "Failed to load bg music!" << std::endl;
	}*/

	// Load font
	pixFont.loadFromFile("res/fonts/PressStart2P-Regular.ttf");
	titleText.setFont(pixFont);

	cursor.setTexture(cursorTex);

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

	// load hud
	hud = std::make_shared<HUD>();
	entityManager.list.push_back(hud);

	// load player + add player to em list
	player = std::make_shared<Player>();
	entityManager.list.push_back(player);

	// load projectiles to pool
	for (int i = 0; i < 20; i++) {
	
		std::shared_ptr<Projectile> p = std::make_shared<Projectile>();
		projectiles.push_back(p);
		entityManager.list.push_back(p);
	}
	
	// load single tele projectile + add tp to em list
	tp = std::make_shared<TeleProjectile>();
	entityManager.list.push_back(tp);

	// Load enemy spikey
	enemySpikey = make_shared<EnemySpikey>(player);

	// level logic -------------------------------------------------------
	switch (currScene) {

		case GameScene::mainMenu:

			currState = GameState::menu;

			// main menu logic here
			titleText.setCharacterSize(100);
			titleText.setString("Mage Madness");
			titleText.setPosition({ (view.getSize().x * .5f) - (titleText.getLocalBounds().width * .5f), 150 });
			
			// add buttons
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
				// load buttons how to play
				std::shared_ptr<Button> but11 = make_shared<Button>("Back", 35, sf::Vector2f(105, 40), 11);
				menuButtonManager.list.push_back(but11);
				buttons.push_back(but11);
			}

			break;

		case GameScene::tutorial_1:

			// tut 1 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 100, 100 };

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
			//player->setTexture(playerTex);
			player->setPosition({ 100, 100 });
			
			// Set enemy turret1 values
			t1 = std::make_shared<EnemyTurret>(player, sf::Vector2f{ 1250, 350 });
			entityManager.list.push_back(t1);

			// Set enemy turret2 values
			t2 = std::make_shared<EnemyTurret>(player, sf::Vector2f{ 900, 350 });
			entityManager.list.push_back(t2);

			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 550,350 });
			// set collectable
			pickup = std::make_shared<PickUp>(sf::Vector2f{ 400, 100 });
			entityManager.list.push_back(pickup);

			break;

		case GameScene::tutorial_2:

			// tut 2 logic here
			currState = GameState::playing;
			break;

		case GameScene::tutorial_3:

			// tut 3 logic here
			currState = GameState::playing;
			break;

		case GameScene::level_1:

			// level 1 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 100, 100 };
			// level load
			ls::loadLevelFile("res/levels/level1.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ initialPlayerPosition});
			// Set enemy turret values
			
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 600,150 });

			break;

		case GameScene::level_2:

			// level 2 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 1050,140 };
			// level load
			ls::loadLevelFile("res/levels/level2.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ initialPlayerPosition });
			// Set enemy turret values
			
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 150, 600 });

			break;
		case GameScene::level_3:

			// level 3 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 100, 450 };
			// level load
			ls::loadLevelFile("res/levels/level3.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ initialPlayerPosition });
			// Set enemy turret values
			
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 150, 600 });

			break;
		case GameScene::level_4:

			// level 4 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 850, 450 };
			// level load
			ls::loadLevelFile("res/levels/level4.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ initialPlayerPosition });
			// Set enemy turret values
			
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 150, 600 });

			break;
		case GameScene::level_5:

			// level 5 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 100, 100 };
			// level load
			ls::loadLevelFile("res/levels/level5.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ initialPlayerPosition });
			// Set enemy turret values
			
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 150, 600 });

			break;
		case GameScene::boss_level_1:

			// boss level 1 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 100, 250 };
			// level load
			ls::loadLevelFile("res/levels/bossLevel1.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ initialPlayerPosition });
			// Set enemy turret values
			
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 150, 600 });
			break;
		case GameScene::boss_level_2:

			// boss level 2 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 700, 150 };
			// level load
			ls::loadLevelFile("res/levels/bossLevel2.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ initialPlayerPosition });
			// Set enemy turret values
			
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 150, 600 });
			break;

		case GameScene::boss_level_3:

			// boss level 3 logic here
			currState = GameState::playing;
			initialPlayerPosition = { 1750, 800 };
			// level load
			ls::loadLevelFile("res/levels/bossLevel3.txt");
			// add tiles to tile list
			for (int i = 0; i < ls::_sprites.size(); i++) {

				tileInfo currTile = ls::_sprites[i];

				std::shared_ptr<Tile> tile = std::make_shared<Tile>(currTile.type, currTile.pos);
				tiles.push_back(tile);
				entityManager.list.push_back(tile);
			}

			// set player values
			player->setTexture(playerTex);
			player->setPosition({ initialPlayerPosition });
			// Set enemy turret values
			
			// Set enemy spikey values
			enemySpikey->setTexture(enemySpikeyTex);
			enemySpikey->setPosition({ 150, 600 });
			break;

		default:
			break;
	}

	// Spikey enemy
	entityManager.list.push_back(enemySpikey);
}

// =========================================================================================================================================================
// Update Loop 
//__________________________________________________________________________________________________________________________________________________________
void Update(RenderWindow& window) {

	// Reset clock, recalculate deltatime
	static Clock clock;
	float dt = clock.restart().asSeconds();

	// get mouse position
	sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// update cursor (mouse & keyboard settings only)
	if (conScheme != 3) {
		cursor.setPosition(mousePosition);
	}
	else {
		MoveCursor(dt);
	}

	// check and consume events ----------------------------------------------------------------------------------------------------------------------------
	Event event;
	while (window.pollEvent(event)) {

		//=============== controller (xbox 360) ================//
		// joy R = xbox Y	|	AxisX = left stick rightleft	//
		// joy X = xbox A	|	AxisU = right stick rightleft	//
		// joy Y = xbox B	|	AxisV = right stick updown		//
		// joy Z = xbox X	|									//
		// joy V = rSh		|	povX = Select					//
		// joy U = lSh		|	povY = Start					//
		//======================================================//
		if (event.type == sf::Event::JoystickMoved) {
			if (event.joystickMove.axis == Joystick::Axis::X) {
				
				if (event.joystickMove.position > 30) {
					// move right
					player->setMoving(false, true);
				}
				else if (event.joystickMove.position < -30) {
					// move left
					player->setMoving(true, false);
				}
				else {
					// stop
					player->setMoving(false, false);
				}
				
			}
			if (event.joystickMove.axis == Joystick::Axis::U) {

				if (event.joystickMove.position > 30) {
					// cursor right
					moveX = cursorSpeed;
				}
				else if (event.joystickMove.position < -30) {
					// cursor left
					moveX = -cursorSpeed;
				}
				else {
					moveX = 0;
				}

			}
			if (event.joystickMove.axis == Joystick::Axis::V) {

				if (event.joystickMove.position > 30) {
					// cursor up
					moveY = cursorSpeed;
				}
				else if (event.joystickMove.position < -30) {
					// cursor down
					moveY = -cursorSpeed;
				}
				else {
					moveY = 0;
				}
			}
		}
		if (event.type == sf::Event::JoystickButtonPressed) {
			if (event.joystickButton.button == Joystick::X) {
				
				// determine if ingame or main menu
				if (currState == GameState::playing) {

					// if ingame jump start
					player->jumpPressed();
				}
				else {

					//if in menu button press
					clickButton(window);
				}
				
			}
			if (event.joystickButton.button == Joystick::V) {
				player->projectilePressed();
			}
		}
		if (event.type == sf::Event::JoystickButtonReleased) {
			if (event.joystickButton.button == Joystick::X) {
				player->jumpReleased();
			}
			if (event.joystickButton.button == Joystick::V) {
				ClickOne(window);
			}
			if (event.joystickButton.button == Joystick::U) {
				ClickTwo(window);
			}
		}
		
		// Use P to take a screenshot
		//if (event.key.code == Keyboard::P)
		//{
		//	// Create a texture from current window
		//	sf::Texture texture;
		//	texture.create(window.getSize());
		//	texture.update(window);

		//	// Copy contents of the texture into an image
		//	sf::Image screenshot = texture.copyToImage();

		//	// Save image to a file
		//	if (screenshot.saveToFile("res/img/screenshot.png"))
		//	{
		//		std::cout << "Screenshot saved" << std::endl;
		//	}
		//	else
		//	{
		//		std::cout << "Failed to save screenshot" << std::endl;
		//	}
		}

		// key pressed events ---------------------------------------------
		if (event.type == sf::Event::KeyPressed)
		{
			// player events if playing
			if (currState == GameState::playing) {

				// prep Jump
				if ((event.key.code == Keyboard::Space && conScheme == 1) ||
					(event.key.code == Keyboard::RControl && conScheme == 2)){

					player->jumpPressed();
				}
			}


			if (event.key.code == Keyboard::Escape) {

				window.close();
			}
		}

		// key released events ----------------------------------------------
		if (event.type == sf::Event::KeyReleased)
		{
			// player events if playing
			if (currState == GameState::playing) {

				if ((event.key.code == Keyboard::Space && conScheme == 1) ||
					(event.key.code == Keyboard::RControl && conScheme == 2)){

					player->jumpReleased();
				}

				// TESTING SCENE SKIP
				if (event.key.code == Keyboard::Right) {

					if (currScene == GameScene::mainMenu) {
						currScene = GameScene::tutorial_1;
					}
					else if(currScene == GameScene::tutorial_1) {
						currScene = GameScene::tutorial_2;
					}
					else if (currScene == GameScene::tutorial_2) {
						currScene = GameScene::tutorial_3;
					}
					else if (currScene == GameScene::tutorial_3) {
						currScene = GameScene::level_1;
					}
					else if (currScene == GameScene::level_1) {
						currScene = GameScene::level_2;
					}
					else if (currScene == GameScene::level_2) {
						currScene = GameScene::level_3;
					}
					else if (currScene == GameScene::level_3) {
						currScene = GameScene::level_4;
					}
					else if (currScene == GameScene::level_4) {
						currScene = GameScene::level_5;
					}
					else if (currScene == GameScene::level_5) {
						currScene = GameScene::boss_level_1;
					}
					else if (currScene == GameScene::boss_level_1) {
						currScene = GameScene::boss_level_2;
					}
					else if (currScene == GameScene::boss_level_2) {
						currScene = GameScene::boss_level_3;
					}
					Reload();
				}
			}

			if (event.key.code == Keyboard::Equal){

				conScheme = 1;
			}
		}

		// mouse pressed events ----------------------------------------------
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left) {

				// determine if ingame or main menu
				if (currState == GameState::playing) {

					// if ingame projectile launch
					player->projectilePressed();
				}
			}
		}
		// mouse released events ----------------------------------------------
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left) {

				// determine if ingame or main menu
				if (currState == GameState::playing) {

					// if ingame projectile launch
					ClickOne(window);
				}
				else {

					//if in menu button press
					clickButton(window);
				}
			}
			if (event.mouseButton.button == sf::Mouse::Right) {

				// TP launch
				ClickTwo(window);
			}
		}

		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	// update entities
	if (currScene != GameScene::mainMenu) {
		entityManager.update(dt);
	}
	else {
		menuButtonManager.update(dt);
	}

	// send info to hud
	hud->JumpX(player->getJcharge());
	hud->SpellX(player->getScharge());

	// check walls for collision ===========================================================================================================================
	for (auto s = begin(tiles); s != end(tiles); ++s) {

		// get bounds for wall
		sf::FloatRect wBounds = (*s)->getGlobalBounds();

		//check projectile collision -----------------------------------------------------------------------------------------------------------------------
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

		// check gravblocks --------------------------------------------------------------------------------------------------------------------------------
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


		// check tp projectile collision --------------------------------------------------------------------------------------------------------------------
		// get projectile bounds
		sf::FloatRect tpBounds = tp->getGlobalBounds();
		// get collision data
		optional tpCol = wBounds.findIntersection(tpBounds);

		if (tp->getState() && tpCol) {

			// get collision rect
			FloatRect colRect = tpCol.value();

			tp->collision(dt, colRect, wBounds, player);
		}


		//check player collision ----------------------------------------------------------------------------------------------------------------------------
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

			if ((*s)->getType() == 4 || (*s)->getType() == 8) {
				// if spiked then kill 
				KillPlayer();
			}
			else if (((*s)->getType() == 2) && (!(*s)->getColliding()) && (colBottom == wallBottom && collision.value().width > collision.value().height)) {
				// if crushed by grav block then kill 
				KillPlayer();
			}
			else {
				// else collide
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

		// check player against collectible
		sf::Vector2f pCentre = { pBounds.left + (pBounds.width * 0.5f), pBounds.top + (pBounds.height * 0.5f) };
		if (pickup->getGlobalBounds().contains(pCentre)) {
			pickup->setActive(false);
			hud->CollectableGained();
		}
	}
	//=======================================================================================================================================================

	// check if turrets can shoot
	if (currState != GameState::menu) {
		TurretShoot(t1);
		TurretShoot(t2);
	}
	
}

//===========================================================================================================================================================
// Draw 
//___________________________________________________________________________________________________________________________________________________________
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

	// draw cursor
	window.draw(cursor);
}

int main() {
	

	// set up view
	view.setSize({ hdGameWidth, hdGameHeight });
	view.setCenter({ view.getSize().x / 2, view.getSize().y / 2 });
	// set initial window properties
	RenderWindow window(VideoMode(desktop.size, desktop.bitsPerPixel), "MageMadness", sf::Style::Fullscreen);
	ResetWindow(window);
	Renderer::initialise(window);

	// hide default mouse cursor
	window.setMouseCursorVisible(false);

	Init();
	Load();
	/*BGmusic.play();*/

	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;
}