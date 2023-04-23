#pragma once
#include "entity_management.h"
#include <SFML/Graphics.hpp>

class HUD : public Entity {
protected:

    int _failCount;
    bool _gotCollectable;
    sf::Text _failText;
    std::string _failCountAsText;

public:
    //Constructor
    HUD();

    //Update
    void Update(const float& dt) override;
    //Render
    void Render(sf::RenderWindow& window) override;

    // methods misc.
    void AddFail();
    void CollectableGained();

};