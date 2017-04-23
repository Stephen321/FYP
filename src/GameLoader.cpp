#include "GameLoader.h"

#include <fstream>
#include "GameData.h"
#include "Map.h"
#include "Constants.h"
#include <string>


using namespace Constants;


GameLoader::GameLoader(const std::string& filepath)
{
	loadData(filepath);
}

std::string GameLoader::loadJSONDATA(std::string const & filename) {
	std::string json;
	std::ifstream file(filename);
	std::string line;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			json.append(line);
		}
	}
	return json;
}

void GameLoader::loadData(const std::string& filepath) {
	GameData& data = GameData::Instance();
	std::string json = loadJSONDATA(filepath + "data.json");
	Document document;
	document.Parse<0>(json.c_str());

	Value::ConstMemberIterator it = document.MemberBegin();
	std::string mapPath = filepath + it->value.GetString();
	++it;
	std::string mapFile = it->value.GetString();
	++it;

	loadMap(mapPath, mapFile);

	data.cameraMoveSpeed = it->value.GetInt();
	++it;

	data.unitTexture.loadFromFile(filepath + it->value.GetString());
	++it;

	data.unitTexture2.loadFromFile(filepath + it->value.GetString());
	++it;

	data.portraitTextureA.loadFromFile(filepath + it->value.GetString());
	++it;

	data.portraitTextureB.loadFromFile(filepath + it->value.GetString());
	++it;

	data.deadTexture.loadFromFile(filepath + it->value.GetString());
	++it;


	data.bulletsTexture.loadFromFile(filepath + it->value.GetString());
	++it;


	data.coverTexture.loadFromFile(filepath + it->value.GetString());
	++it;

	data.menuTexture.loadFromFile(filepath + it->value.GetString());
	++it;

	data.gameOverTexture.loadFromFile(filepath + it->value.GetString());
	++it;
	
	data.font.loadFromFile(filepath + it->value.GetString());
	++it;
	
	data.unitMoveSpeed = it->value.GetFloat();
	++it;

	data.bulletPoolSize = it->value.GetInt();
	++it;

	data.bulletTexture.loadFromFile(filepath + it->value.GetString());
	++it;

	data.ammoPoolSize = it->value.GetInt();
	++it;

	data.ammoPoolTexture.loadFromFile(filepath + it->value.GetString());
	++it;

	//weapons
	Value::ConstMemberIterator& weaponIt = it->value.MemberBegin();
	data.pistolDamage = weaponIt->value.GetObject()["damage"].GetInt();
	data.pistolMoveSpeed = weaponIt->value.GetObject()["bullet_move_speed"].GetFloat();
	data.pistolRange = weaponIt->value.GetObject()["range"].GetInt();
	data.pistolReloadTime = weaponIt->value.GetObject()["reload_time"].GetFloat();
	data.pistolAmmo = weaponIt->value.GetObject()["ammo"].GetFloat();
	++weaponIt;

	data.shotgunDamage = weaponIt->value.GetObject()["damage"].GetInt();
	data.shotgunMoveSpeed = weaponIt->value.GetObject()["bullet_move_speed"].GetFloat();
	data.shotgunRange = weaponIt->value.GetObject()["range"].GetInt();
	data.shotgunReloadTime = weaponIt->value.GetObject()["reload_time"].GetFloat();
	data.shotgunAmmo = weaponIt->value.GetObject()["ammo"].GetFloat();
	++weaponIt;

	data.rifleDamage = weaponIt->value.GetObject()["damage"].GetInt();
	data.rifleMoveSpeed = weaponIt->value.GetObject()["bullet_move_speed"].GetFloat();
	data.rifleRange = weaponIt->value.GetObject()["range"].GetInt();
	data.rifleReloadTime = weaponIt->value.GetObject()["reload_time"].GetFloat();
	data.rifleAmmo = weaponIt->value.GetObject()["ammo"].GetFloat();
	++weaponIt;

	data.sniperDamage = weaponIt->value.GetObject()["damage"].GetInt();
	data.sniperMoveSpeed = weaponIt->value.GetObject()["bullet_move_speed"].GetFloat();
	data.sniperRange = weaponIt->value.GetObject()["range"].GetInt();
	data.sniperReloadTime = weaponIt->value.GetObject()["reload_time"].GetFloat();
	data.sniperAmmo = weaponIt->value.GetObject()["ammo"].GetFloat();
	++weaponIt;

}

void GameLoader::loadMap(std::string const & mapPath, std::string const & mapFile)
{
	std::string json = loadJSONDATA(mapPath + mapFile);
	Document document;
	document.Parse<0>(json.c_str());

	m_width = document["width"].GetInt();
	m_height = document["height"].GetInt();

	const Value& layers = document["layers"];
	for (Value::ConstValueIterator it = layers.Begin(); it != layers.End(); it++)
	{
		const Value& layer = (*it);
		if (layer["type"].GetString()[0] == 't')
		{
			const Value& tilesetObj = document["tilesets"][0]; //only one tileset
			loadTiles(tilesetObj, layer, mapPath);
		}
		else //object layer
		{
			if (layer["name"].GetString()[0] == 'P')
			{
				loadGraph(layer);
			}
		}
	}
}

void GameLoader::loadGraph(const Value & layer)
{
	GameData& data = GameData::Instance();

	const Value::ConstArray& objects = layer["objects"].GetArray();

	Graph graph(objects.Size());


	std::vector<std::pair<int, int>> arcs;
	for (int i = 0; i < objects.Size(); i++)
	{
		const Value& properties = objects[i]["properties"];
		int index = std::stoi(objects[i]["name"].GetString());

		sf::Vector2f position;
		position.x = objects[i]["x"].GetFloat() + (TILE_SIZE * 0.5f);
		position.y = objects[i]["y"].GetFloat() + (TILE_SIZE * 0.5f);

		int coverType = properties["coverType"].GetInt();
		for (int j = 0; j < properties["neighbours"].Size(); j++)
		{
			int to = properties["neighbours"][j].GetInt();
			arcs.push_back(std::pair<int, int>(index, to)); 
			arcs.push_back(std::pair<int, int>(to, index));
		}
		graph.addNode(index, position, coverType);
	}

	//cover nodes have arcs to neighbours but neighbours dont have arcs to them. cant pathfinding through cover nodes
	graph.addArcs(arcs);

	data.map.addGraph(graph);
}

void GameLoader::loadTiles(const Value& tilesetObj, const Value& layer, std::string const & mapPath)
{
	GameData& data = GameData::Instance();
	sf::VertexArray vertices;
	sf::Texture tileset;

	std::string tilesetPath = mapPath + tilesetObj["image"].GetString();
	tileset.loadFromFile(tilesetPath);
	int firstgid = tilesetObj["firstgid"].GetInt();
	int columns = tilesetObj["columns"].GetInt();

	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(m_width * m_height * 4); //4 vertices for each tile

	int collisionGid = layer["properties"]["collision_gid"].GetInt();
	std::vector<Collidable> collidables;

	const Value& arrayData = layer["data"];
	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			int gid = arrayData[i + j * m_width].GetInt();
			if (gid == collisionGid)
			{
				sf::FloatRect rect;
				rect.left = i * TILE_SIZE;
				rect.top = j * TILE_SIZE;
				rect.width = TILE_SIZE;
				rect.height = TILE_SIZE;
				Collidable collidable(ObjectType::Wall);
				collidable.setRect(rect);
				collidables.push_back(collidable);
			}
			gid -= firstgid; //from 0-3 instead of 1-4

							 // find its position in the tileset texture
			int tu = gid % (tileset.getSize().x / TILE_SIZE);
			int tv = gid / (tileset.getSize().x / TILE_SIZE);

			sf::Vertex* quad = &vertices[(i + j * m_width) * 4];

			//top left
			//top right
			//bottom right
			//bottom left

			quad[0].position = sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
			quad[1].position = sf::Vector2f((i + 1) * TILE_SIZE, j * TILE_SIZE);
			quad[2].position = sf::Vector2f((i + 1) * TILE_SIZE, (j + 1) * TILE_SIZE);
			quad[3].position = sf::Vector2f(i * TILE_SIZE, (j + 1) * TILE_SIZE);

			quad[0].texCoords = sf::Vector2f(tu * TILE_SIZE, tv * TILE_SIZE);
			quad[1].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, tv * TILE_SIZE);
			quad[2].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, (tv + 1) * TILE_SIZE);
			quad[3].texCoords = sf::Vector2f(tu * TILE_SIZE, (tv + 1) * TILE_SIZE);
		}
	}
	data.map.addVertices(vertices, tileset);
	data.map.addCollidables(collidables);
}