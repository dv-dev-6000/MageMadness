//entity_management.cpp
#include "entity_management.h"
#include <queue>

using namespace std;
using namespace sf;

// EntityManager -------------------------------------------------
void EntityManager::update(const float& dt) {
	for (auto& e : list) {
		e->Update(dt);
	}
}

void EntityManager::render(sf::RenderWindow& window) {
	for (const auto& e : list) {
		e->Render(window);
	}
}


// Entity Class --------------------------------------------------
Entity::Entity() {};
void Entity::Update(const float& dt) { }
void Entity::Render(sf::RenderWindow& window) { }


// Renderer ------------------------------------------------------
static queue<const Drawable*> sprites;
static RenderWindow* rw;

void Renderer::initialise(sf::RenderWindow& r) { rw = &r; }

sf::RenderWindow& Renderer::getWindow() { return *rw; }

void Renderer::shutdown() {
	while (!sprites.empty())
		sprites.pop();
}

void Renderer::update(const double&) {}

void Renderer::render() {
	if (rw == nullptr) {
		throw("No render window set! ");
	}
	while (!sprites.empty()) {
		rw->draw(*sprites.front());
		sprites.pop();
	}
}

void Renderer::queue(const sf::Drawable* s) { sprites.push(s); }
