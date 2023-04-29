#include <filesystem>
#include <fstream>
#include <iostream>
#include "data.h"

using namespace std;
namespace fs = std::filesystem;

Data::Data() {

	_folderPath = "";
	_folderPath.append(getenv("APPDATA"));
	_folderPath.append("\\MageMadness");

	_filePath = _folderPath;
	_filePath.append("\\SaveData.txt");
}

/// <summary>
/// Checks to see if the MageMadness folder exists in folder location (User/AppData/Roaming), if it doesnt exist it creates the folder.
/// </summary>
/// <returns>True = Folder Located, False = Failed To Create Folder</returns>
bool Data::CheckCreateSavesFolder() {

	if (fs::exists(_folderPath) && fs::is_directory(_folderPath)) {
		// folder exists at location 
		return true;
	}
	else {
		// not found at location so create
		if (fs::create_directory(_folderPath)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool Data::CheckCreateSaveFile() {

	// check if file exists
	if (fs::exists(_filePath)) {
		// file exists at location 
		test = "file exists";
		return true;
	}
	else {
		// not found at location so create

		// Create and open a text file
		ofstream MyFile(_filePath);
		// Write to the file
		MyFile << "0\n0";
		// Close the file
		MyFile.close();

		// check again after file created
		if (fs::exists(_filePath)) {
			return true;
		}
		else {
			return false;
		}
	}
}

void Data::UpdateSave(int level, int fails) {

	// convert save data to string
	std::string data = std::to_string(level) + "\n" + std::to_string(fails);

	// open a text file
	ofstream MyFile(_filePath, ofstream::trunc);
	// overwrite the file
	MyFile << data;
	// Close the file
	MyFile.close();
}

sf::Vector2i Data::LoadProgress() {

	int level;
	int fails;
	
	ifstream MyFile(_filePath);
	std::string MyString;
	
	getline(MyFile, MyString);
	level = stoi(MyString);
	getline(MyFile, MyString);
	fails = stoi(MyString);
	
	sf::Vector2i data = { level, fails };
	
	return data;
}