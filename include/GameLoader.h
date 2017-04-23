#pragma once

#include "SFML/Graphics.hpp" 
//#include <dirent.h>
#include "rapidjson\document.h"

using namespace rapidjson;

class GameLoader {
public:
	GameLoader(const std::string& filepath);
private:
	void loadData(const std::string& filename);
	std::string loadJSONDATA(std::string const & filepath);
	void loadMap(std::string const & mapPath, std::string const & mapFile);
	void loadTiles(const Value& tilesetObj, const Value& layer, std::string const & mapPath);
	void loadGraph(const Value& layer);

	int m_width;
	int m_height;
};