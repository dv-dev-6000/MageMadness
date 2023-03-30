//entity.h
#pragma once
#include <SFML/Graphics.hpp>

class Entity : public sf::Sprite {
protected:
    sf::IntRect _sprite;
    //Default constructor is hidden
    Entity();
public:
    //Constructor that takes a sprite
    explicit Entity(sf::IntRect ir);
    //Pure virtual deconstructor -- makes this an abstract class and avoids undefined behaviour!
    virtual ~Entity() = 0;
    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& dt);
};