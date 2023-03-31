#include <SFML/Graphics.hpp>
#include <iostream>
#include "cloud.h"

using namespace sf;
using namespace std;


// Game Variables =======================================================================================================

// get desktop resolution info
VideoMode desktop = VideoMode::getDesktopMode();
// create view object (used to scale content with window size and adjust aspect ratio) 
sf::View view;

// textures 
sf::Texture background;
sf::Texture back2;
//sprites
sf::Sprite bkSprite;
sf::Sprite bk2Sprite;

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
	// set test image as sprite texture
	bkSprite.setTexture(background);
	bk2Sprite.setTexture(back2);
	// set values for source rect
	bk2Sprite.setTextureRect(IntRect(Vector2(0, 0), Vector2(1920, 1080)));
	bk2Sprite.setPosition({ 0,0 });
	bkSprite.setTextureRect(IntRect(Vector2(0, 0), Vector2(800, 800)));
	bkSprite.setPosition({ 0,0 });
	

}

/// <summary>
/// reload content called to transition between scenes
/// </summary>
void Reload() {

	


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
				ResetWindow(window);
			}
		}

		if (event.type == sf::Event::KeyPressed)
		{
			// if S key pressed
			if (event.key.code == Keyboard::S) {

				bkSprite.setPosition(bkSprite.getPosition() + Vector2f(1, 1));
			}
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

	// move
	if (Keyboard::isKeyPressed(Keyboard::Down)) {

		if (bkSprite.getPosition().y < (1080 - 800)) {

			bkSprite.setPosition(bkSprite.getPosition() + Vector2f(1, 1));
		}
		
	}

}



// Draw =================================================================================================================

void Render(RenderWindow& window) {
	// Draw Everything
	window.draw(bk2Sprite);
	window.draw(bkSprite);
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