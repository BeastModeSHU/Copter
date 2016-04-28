#include "Map.h"

using namespace GameConstants::Map;

sf::Vector2i Map::DIRECTIONS[8] = { sf::Vector2i(-1, -1), sf::Vector2i(0, -1), sf::Vector2i(1, -1),
sf::Vector2i(1, 0), sf::Vector2i(1, 1), sf::Vector2i(0, 1),
sf::Vector2i(-1, 1), sf::Vector2i(-1, 0) };

Map::Map()
{
	vertexArray_.setPrimitiveType(sf::Quads);
	vertexArray_.resize(MAP_WIDTH * MAP_HEIGHT * 4);
	srand(static_cast<unsigned>(time(0)));
}

Map::~Map()
{

}

//Public functions
void Map::generateMap()
{
	setupBlockedMap();
	initVertArray();
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	if (p_tileset_ != nullptr)
		states.texture = p_tileset_;
	target.draw(vertexArray_, states);
}

bool Map::isTerrainCollision(const sf::FloatRect& collider) const
{
	return(isCollidingWithMap(collider));
}

//Private functions
void Map::setupBlockedMap()
{ //Function to randomly generate a map

	int minY(0), maxY(0);

	minY = getMinY(minY); //set the lowest y-value to dip to 
	maxY = getMaxY(maxY); //set the heighest y-value to rise to 

	fillMapBlocked(); //Fill the map as blocked tiles 

	int lastY = minY + maxY / 2; //
	playerStart_.x = 0;
	playerStart_.y = lastY * TILESIZE;
	for (int j(0); j < 4; ++j)
	{
		if (lastY + j < MAP_HEIGHT - 1)
			blockedMap_[0][lastY + j] = FREE_TILE;

		if (lastY - j > 0)
			blockedMap_[0][lastY - j] = FREE_TILE;
	}

	blockedMap_[0][lastY] = FREE_TILE;

	int direc = 1;

	for (int i(1); i < MAP_WIDTH; ++i)
	{
		int y = lastY + direc;

		if (y < minY)
		{
			minY = getMinY(minY);
			direc *= -1;
			y = lastY + direc;
		}
		else if (y > maxY)
		{
			maxY = getMaxY(y);
			direc *= -1;
			y = lastY + direc;
		}

		for (int j(0); j < 4; ++j)
		{
			if (y + j < MAP_HEIGHT - 1)
				blockedMap_[i][y + j] = FREE_TILE;

			if (y - j > 0)
				blockedMap_[i][y - j] = FREE_TILE;
		}

		lastY = y;
	}

	for (size_t i(1); i < MAP_WIDTH - 1; ++i) //Redundant to check the first and last tile of x 
	{
		for (size_t j(1); j < MAP_HEIGHT - 1; ++j) //Redundant to check first and last tile of y
		{

			if (blockedMap_[i - 1][j] == FREE_TILE && blockedMap_[i][j] == BLOCKED_TILE && blockedMap_[i + 1][j] == FREE_TILE)
				blockedMap_[i][j] = FREE_TILE;

			if (blockedMap_[i - 1][j] == BLOCKED_TILE && blockedMap_[i][j] == BLOCKED_TILE && blockedMap_[i + 1][j] == BLOCKED_TILE
				&& blockedMap_[i][j + 1] == FREE_TILE);
		}

	}
}

void Map::fillMapBlocked()
{
	for (size_t i(0); i < MAP_WIDTH; ++i)
		for (size_t j(0); j < MAP_HEIGHT; ++j)
			blockedMap_[i][j] = BLOCKED_TILE;
}

int Map::getMinY(int val)const
{
	int a(0);

	while (a < 1 || a > 5 || a == val)
		a = rand() % MAP_HEIGHT;

	return(a);
}

int Map::getMaxY(int val) const
{
	int a(0);

	while (a < 6 || a > 10 || a == val)
		a = rand() % MAP_HEIGHT;

	return(a);
}

void Map::initVertArray()
{

	for (size_t i(0); i < MAP_WIDTH; ++i)
	{
		for (size_t j(0); j < MAP_HEIGHT; ++j)
		{
			const int tileID(i + j * MAP_WIDTH);

			sf::Vertex* tile = &vertexArray_[tileID * 4];

			tile[0].position = sf::Vector2f(i * TILESIZE, j * TILESIZE);
			tile[1].position = sf::Vector2f((i + 1) * TILESIZE, j * TILESIZE);
			tile[2].position = sf::Vector2f((i + 1) * TILESIZE, (j + 1) * TILESIZE);
			tile[3].position = sf::Vector2f(i * TILESIZE, (j + 1) * TILESIZE);

			switch (blockedMap_[i][j])
			{
			case BLOCKED_TILE:
				tile[0].color = sf::Color::Cyan;
				tile[1].color = sf::Color::Cyan;
				tile[2].color = sf::Color::Cyan;
				tile[3].color = sf::Color::Cyan;
				break;
			case FREE_TILE:
				tile[0].color = sf::Color::Green;
				tile[1].color = sf::Color::Green;
				tile[2].color = sf::Color::Green;
				tile[3].color = sf::Color::Green;
				break;
			}
		}
	}
}

bool Map::isCollidingWithMap(const sf::FloatRect& collider) const
{
	sf::Vector2i pos, tileToCheck;
	sf::FloatRect r; 
	r.width = TILESIZE;
	r.height = TILESIZE;

	pos.x = static_cast<int> (collider.left / TILESIZE);
	pos.y = static_cast<int> (collider.top / TILESIZE);

	for (size_t i(0); i < DIREC_SIZE; ++i)
	{
		tileToCheck = pos + DIRECTIONS[i];

		if (tileToCheck.x >= 0 && tileToCheck.x < MAP_WIDTH &&
			tileToCheck.y >= 0 && tileToCheck.y < MAP_HEIGHT)
		{
			r.left = tileToCheck.x * TILESIZE; 
			r.top = tileToCheck.y * TILESIZE;

			if (blockedMap_[tileToCheck.x][tileToCheck.y] == BLOCKED_TILE && r.intersects(collider))
				return (true);
		}
	}

	return(false);
}
