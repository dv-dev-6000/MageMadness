//entity_management.h
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Entity : public sf::Sprite{
protected:
    Entity();
public:

    virtual ~Entity() = default;

	virtual void Update(const float& dt);
	virtual void Render(sf::RenderWindow& window);
};

struct EntityManager {
	std::vector<std::shared_ptr<Entity>> list;
	void update(const float& dt);
	void render(sf::RenderWindow& window);
};

namespace Renderer {
    void initialise(sf::RenderWindow&);
    sf::RenderWindow& getWindow();

    void shutdown();
    void update(const double&);
    void queue(const sf::Drawable* s);
    void render();
};