#include "GameData.h"

#include "Constants.h"

using namespace Constants;

GameData::GameData() : gameTime(0.f), aDiff(DIFF_EASY), bDiff(DIFF_EASY), useTimeToEnd(false), debug(false) {}

GameData& GameData::Instance() 
{
	static GameData gd;
	return gd;
}
