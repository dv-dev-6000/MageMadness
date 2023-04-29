//cloud.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// constant vatiables
constexpr uint16_t hdGameWidth = 1920;
constexpr uint16_t hdGameHeight = 1080;

extern sf::Texture tileTex;
extern sf::Texture breakTileTex;
extern sf::Texture gravTileTex;
extern sf::Texture spikeTileTex;
extern sf::Texture bossBlockTileTex;
extern sf::Texture area1BlockTileTex;
extern sf::Texture endBlockTileTex;
extern sf::Texture upDownSpikesTex;
//extern sf::Texture endTileTex;

extern sf::Texture playerTex;
extern sf::Texture whiteBallTex;
extern sf::Texture buttonTex;

extern sf::Texture hudBaseTex;
extern sf::Texture hudOverTex;
extern sf::Texture pickupTex;

extern sf::Font pixFont;

// Enemies
extern sf::Texture enemyTurTex;
extern sf::Texture enemySpikeyTex;

// control state
extern int conScheme;
// cursor
extern sf::Sprite cursor;




