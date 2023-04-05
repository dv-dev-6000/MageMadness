#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::Sprite {
protected:

public:

    //Constructor
    Player();

    //Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& dt);
};