#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <unordered_map>
#include "SFML\Graphics\Texture.hpp"

class GameData {
public:
	static GameData& getInstance();
	// delete copy and move constructors and assign operators
	GameData(GameData const&) = delete;             // Copy construct
	GameData(GameData&&) = delete;                  // Move construct
	GameData& operator=(GameData const&) = delete;  // Copy assign
	GameData& operator=(GameData &&) = delete;      // Move assign
private:
	GameData();
};
#endif