#pragma once
#include <SFML/Graphics.hpp>

class Data {
protected:

    std::string _folderPath;
    std::string _filePath;

public:

    std::string test;

    //Constructor
    Data();

    //Methods
    bool CheckCreateSavesFolder();
    bool CheckCreateSaveFile();
    void UpdateSave(int level, int fails);
    sf::Vector2i LoadProgress();
};