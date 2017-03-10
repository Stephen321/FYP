#ifndef GAMELOADER_H
#define GAMELOADER_H

#include "rapidjson\document.h"
#include <memory>
#include <fstream>
#include "SFML/Graphics.hpp" 
#include "GameData.h"
//#include <dirent.h>

using namespace rapidjson;

class GameLoader {
public:
	GameLoader(const std::string& filePath);
private:
	rapidjson::Document m_document;
	std::string m_JSONData;
	void loadData(const std::string& filePath);
	void loadJSONDATA(std::string const & filename);
};

#endif 