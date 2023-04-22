//LevelSystem.cpp
#include "LevelSystem.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace sf;

std::unique_ptr<LevelSystem::TILE[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
Vector2f LevelSystem::_offset(0.0f, 0.0f);

float LevelSystem::_tileSize(100.f);
vector<tileInfo> LevelSystem::_sprites;

std::map<LevelSystem::TILE, sf::Color> LevelSystem::_colours{ {WALL, Color::White}, {END, Color::Red} };

size_t LevelSystem::getHeight() {
    return _height;
}

size_t LevelSystem::getWidth() {
    return _width;
}

void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
    _tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;

    // Load in file to buffer
    ifstream f(path);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    }
    else {
        throw string("Couldn't open level file: ") + path;
    }

    std::vector<TILE> temp_tiles;
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        switch (c) {
        case 'w':
            temp_tiles.push_back(WALL);
            break;
        case 'g':
            temp_tiles.push_back(GRAVBLOCK);
            break;
        case 'e':
            temp_tiles.push_back(END);
            break;
        case ' ':
            temp_tiles.push_back(EMPTY);
            break;
        case 'b':
            temp_tiles.push_back(BREAKBLOCK);
            break;
        case 's':
            temp_tiles.push_back(SPIKE);
            break;
        case 'q':
            temp_tiles.push_back(BOSSBLOCK);
            break;
        case 'a':
            temp_tiles.push_back(AREA1BLOCK);
            break;
        case 'u':
            temp_tiles.push_back(UPDOWNSPIKE);
            break;
        case '\n':      // end of line
            if (w == 0) { // if we haven't written width yet
                w = i;      // set width
            }
            h++; // increment height
            break;
        default:
            cout << c << endl; // Don't know what this tile type is
        }
    }
    if (temp_tiles.size() != (w * h)) {
        throw string("Can't parse level file") + path;
    }
    _tiles = std::make_unique<TILE[]>(w * h);
    _width = w; //set static class vars
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
    buildSprites();
}

Vector2f LevelSystem::getTilePosition(Vector2ul p) {
    return (Vector2f(p.x, p.y) * _tileSize);
}

LevelSystem::TILE LevelSystem::getTile(Vector2ul p) {
    if (p.x > _width || p.y > _height) {
        throw string("Tile out of range: ") + to_string(p.x) + "," + to_string(p.y) + ")";
    }
    return _tiles[(p.y * _width) + p.x];
}

void LevelSystem::buildSprites() {
    
    _sprites.clear();

    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) {
            
            tileInfo s;
            s.pos = (getTilePosition({ x, y }));

            ls::TILE currTile = getTile({ x, y });
            switch (currTile) {

                case WALL:
                    s.type = 1;
                    _sprites.push_back(move(s));
                    break;

                case GRAVBLOCK:
                    s.type = 2;
                    _sprites.push_back(move(s));
                    break;

                case BREAKBLOCK:
                    s.type = 3;
                    _sprites.push_back(move(s));
                    break;

                case SPIKE:
                    s.type = 4;
                    _sprites.push_back(move(s));
                    break;

                case END:
                    s.type = 5;
                    _sprites.push_back(move(s));
                    break;

                case BOSSBLOCK:
                    s.type = 6;
                    _sprites.push_back(move(s));
                    break;

                case AREA1BLOCK:
                    s.type = 7;
                    _sprites.push_back(move(s));
                    break;

                case UPDOWNSPIKE:
                    s.type = 8;
                    _sprites.push_back(move(s));
                    break;

                default:
                    break;

            }
        }
    }
}

//void LevelSystem::Render(RenderWindow& window) {
//    for (size_t i = 0; i < _width * _height; ++i) {
//        //window.draw(*_sprites[i]);
//    }
//}