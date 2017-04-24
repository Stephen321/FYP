#pragma once

#include <unordered_map>
#include "SFML\Graphics\Texture.hpp"
#include "Map.h"

//stores data used in the game that has been loaded in on application start
class GameData {
public:
	static GameData& Instance();

	// delete copy and move constructors and assign operators
	GameData(GameData const&) = delete;             // Copy construct
	GameData(GameData&&) = delete;                  // Move construct
	GameData& operator=(GameData const&) = delete;  // Copy assign
	GameData& operator=(GameData &&) = delete;      // Move assign

	Map map;
	int cameraMoveSpeed;
	sf::Texture unitTexture;
	sf::Texture unitTexture2;
	sf::Texture portraitTextureA;
	sf::Texture portraitTextureB;
	sf::Texture deadTexture;
	sf::Texture bulletsTexture;
	sf::Texture coverTexture;
	sf::Texture menuTexture;
	sf::Texture gameOverTexture;
	sf::Font font;
	float unitMoveSpeed;
	int bulletPoolSize;
	sf::Texture bulletTexture;
	int ammoPoolSize;
	sf::Texture ammoPoolTexture;

	//bullet damage and speeds
	int pistolDamage;
	float pistolMoveSpeed;
	int pistolRange;
	float pistolReloadTime;
	int pistolAmmo;
	int shotgunDamage;
	float shotgunMoveSpeed;
	int shotgunRange;
	float shotgunReloadTime;
	int shotgunAmmo;
	int rifleDamage;
	float rifleMoveSpeed;
	int rifleRange;
	float rifleReloadTime;
	int rifleAmmo;
	int sniperDamage;
	float sniperMoveSpeed;
	int sniperRange;
	float sniperReloadTime;
	int sniperAmmo;

	int teamADeaths;
	int teamBDeaths;
	int teamANodes;
	int teamBNodes;
	int teamAShots;
	int teamBShots;
	float gameTime;
	std::string aDiff;
	std::string bDiff;
	bool useTimeToEnd;
	bool debug;
	bool testing;
	bool firstTest;
	int testCount;

	struct SumTest {
		float gameTime;
		int deaths;
		int kills;
		int shots;
		int nodes;
	};
	SumTest sumsA;
	SumTest sumsB;

	float timeMultiplier;
private:
	GameData();
};