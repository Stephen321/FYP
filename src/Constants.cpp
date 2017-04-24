#include "Constants.h"

#include "ObjectType.h"

namespace Constants
{
	const int UNIT_COL_FILTERS = (int)ObjectType::Wall | (int)ObjectType::Bullet | (int)ObjectType::Ammo;
	const int COVER_COL_FILTERS = (int)ObjectType::Unit | (int)ObjectType::Bullet;
	const int WALL_COL_FILTERS = (int)ObjectType::Unit | (int)ObjectType::Bullet;
	const int BULLET_COL_FILTERS = (int)ObjectType::Unit | (int)ObjectType::Wall;
	const int AMMO_COL_FILTERS = (int)ObjectType::Unit;
	
	const float RESPAWN_TIME = 2.f;
	const int TILE_SIZE = 64;


	const std::string COVER_WEST = "West";
	const std::string COVER_NORTH = "North";
	const std::string COVER_EAST = "East";
	const std::string COVER_SOUTH = "South";

	const int TEAM_A = 0;
	const int TEAM_B = 1;

	const int ID_TEXT_Y_OFFSET = 42;
	const std::string DIFF_EASY = "Random";
	const std::string DIFF_HARD = "GOAP";
}