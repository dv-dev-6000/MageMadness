#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;


// Game Variables =======================================================================================================

// get desktop resolution info
VideoMode desktop = VideoMode::getDesktopMode();

sf::Texture background;
sf::Sprite bkSprite;

// Game Methods ===========================================================================================================



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
	bkSprite.setTextureRect(IntRect(Vector2(0, 0), Vector2(1920, 1080)));

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

	// set window properties
	RenderWindow window(VideoMode(desktop.size, desktop.bitsPerPixel), "MageMadness", sf::Style::Fullscreen);
	// disable repeated key events
	window.setKeyRepeatEnabled(false);
	// enable or disable vsync - do at start of game, or via options menu
	window.setVerticalSyncEnabled(false);
	// cap fps 
	window.setFramerateLimit(60);
	
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