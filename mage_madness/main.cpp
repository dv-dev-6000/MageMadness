#include <SFML/Graphics.hpp>
#include <iostream>
#include "cloud.h"

using namespace sf;
using namespace std;


// Game Variables =======================================================================================================

// get desktop resolution info
VideoMode desktop = VideoMode::getDesktopMode();
// create view variable
sf::View view;

// textures & sprites
sf::Texture background;
sf::Sprite bkSprite;

// Game Methods ===========================================================================================================

void ResetWindow(RenderWindow& window) {

	// disable repeated key events
	window.setKeyRepeatEnabled(false);
	// enable or disable vsync - do at start of game, or via options menu
	window.setVerticalSyncEnabled(false);
	// cap fps 
	window.setFramerateLimit(60);
	// set window view
	window.setView(view);
}


// Initialise ===========================================================================================================

void Init() {

	// initialise vars
	
}

// Load Content =========================================================================================================

void Load() {

	// load test image
	if (!background.loadFromFile("res/img/testBackground.png")) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	// set test image as sprite texture
	bkSprite.setTexture(background);
	// set values for source rect
	bkSprite.setTextureRect(IntRect(Vector2(0, 0), Vector2(800, 800)));
	bkSprite.setPosition({ 0,0 });

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

				window.create(VideoMode({ 1280,720 }, desktop.bitsPerPixel), "MageMadness", sf::Style::Default);
				ResetWindow(window);
			}
			if (event.key.code == Keyboard::F2) {

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
		bkSprite.setPosition(bkSprite.getPosition() + Vector2f(1, 1));
	}

}



// Draw =================================================================================================================

void Render(RenderWindow& window) {
	// Draw Everything
	window.draw(bkSprite);
}

int main() {
	
	// set up view
	view.setSize({ defaultGameWidth, defaultGameHeight });
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