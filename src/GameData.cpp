#include "GameData.h"

#include "Constants.h"

using namespace Constants;

GameData::GameData()
	: gameTime(0.f)
	, aDiff(DIFF_EASY)
	, bDiff(DIFF_EASY)
	, useTimeToEnd(true)
	, debug(false)
	, timeMultiplier(1.f)
	, teamAShots(0)
	, teamBShots(0)
	, teamANodes(0)
	, teamBNodes(0)
	, firstTest(true)
	, testCount(0)
{
	testing = false;
}

GameData& GameData::Instance() 
{
	static GameData gd;
	return gd;
}
