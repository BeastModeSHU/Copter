#include "Map.h"
#include <iostream>
using namespace GameConstants::Map;

sf::Vector2i Map::DIRECTIONS[8] = {
	sf::Vector2i(-1, -1), sf::Vector2i(0, -1), sf::Vector2i(1, -1),
	sf::Vector2i(1, 0), sf::Vector2i(1, 1), sf::Vector2i(0, 1),
	sf::Vector2i(-1, 1), sf::Vector2i(-1, 0)
};

Map::Map()
{
	red_ = sf::Vector3f(255, 0, 0);
	blue_ = sf::Vector3f(0, 255, 0);
	green_ = sf::Vector3f(0, 0, 255);
	startColour_ = red_;
	endColour_ = blue_;
	needNewColour_ = false;
	t_ = 0;

	vertexArray_.setPrimitiveType(sf::Quads);
	vertexArray_.resize(MAP_WIDTH * MAP_HEIGHT * 4);
	srand(static_cast<unsigned>(time(0)));

	if (!tileset_.loadFromFile("res//tiles.png"))
	{
	}
}

Map::~Map()
{

}

//Public functions
void Map::generateMap()
{
	vertexArray_.clear();
	vertexArray_.resize(MAP_WIDTH * MAP_HEIGHT * 4);

	setupBlockedMap();
	initVertArray();
}

void Map::lerpColours(float delta, const sf::Vector2f& playerPos, const int g)
{
	t_ += (delta / 2) * g;
	if (needNewColour_)
	{
		startColour_ = endColour_;
		sf::Vector3f newColour_;
		bool found(false);
		while (!found)
		{
			int randNum = rand() % 3;
			switch (randNum)
			{
			case 0:
				if (endColour_ != red_)
				{
					newColour_ = red_;
					found = true;
				}
				break;
			case 1:
				if (endColour_ != blue_)
				{
					newColour_ = blue_;
					found = true;
				}
				break;
			case 2:
				if (endColour_ != green_)
				{
					newColour_ = green_;
					found = true;
				}
				break;

			}
		}

		endColour_ = newColour_;
		needNewColour_ = false;
	}
	else
	{
		sf::Vector3f colour = startColour_ + t_ * (endColour_ - startColour_);
		currentCol_ = colour;
		sf::Vector2i pos(playerPos.x / TILESIZE, playerPos.y / TILESIZE);

		//for (size_t i(pos.x - 10); i < pos.x + 10; ++i)
		for (size_t i(0); i < MAP_WIDTH; ++i)
		{
			for (size_t j(0); j < MAP_HEIGHT; ++j)
			{
				const int tileID(i + j * MAP_WIDTH);

				sf::Vertex* tile = &vertexArray_[tileID * 4];
				switch (blockedMap_[i][j])
				{
				case BLOCKED_TILE:
					tile[0].color = sf::Color(colour.x, colour.y, colour.z);
					tile[1].color = sf::Color(colour.x, colour.y, colour.z);
					tile[2].color = sf::Color(colour.x, colour.y, colour.z);
					tile[3].color = sf::Color(colour.x, colour.y, colour.z);

					break;
				}

			}
		}

		if (g == 1)
		{
			if (colour == endColour_ || t_ > 1)
			{
				needNewColour_ = true;
				t_ = 0;
			}

		}
		else
		{
			if (colour == startColour_ || t_ < -1)
			{
				needNewColour_ = true;
				t_ = 0;
			}
		}

	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	//if (p_tileset_ != nullptr)
	states.texture = &tileset_;
	target.draw(vertexArray_, states);
}

bool Map::isTerrainCollision(const sf::FloatRect& collider) const
{
	return(isCollidingWithMap(collider));
}

//Private functions
void Map::setupBlockedMap()
{ //Function to randomly generate a map

	const int X_RANGE(3);
	int minY(0), maxY(0);

	minY = getMinY(minY); //set the lowest y value to dip to 
	maxY = getMaxY(maxY); //set the heighest y value to rise to 

	fillMapBlocked(); //Fill the map as blocked tiles 

	int lastY = minY + maxY / 2; //set the start location of the crawler to the mid-point between min and max y 

	playerStart_.x = 0; //Set the player's x to 0
	playerStart_.y = lastY * TILESIZE; //Set their y location to the start y value

	//set the start tile to unblocked
	blockedMap_[0][lastY] = FREE_TILE;

	//pad out the area around the start tile so that the player can fit through
	for (int j(0); j < 4; ++j)
	{
		if (lastY + j < MAP_HEIGHT - 1)
			blockedMap_[0][lastY + j] = FREE_TILE;

		if (lastY - j > 0)
			blockedMap_[0][lastY - j] = FREE_TILE;
	}


	int direc;
	//randomly pick whether to travel upwards or downwards

	(rand() % 100) + 1 > 50 ? direc = 1 : direc = -1;


	int xDistance((rand() % X_RANGE) + 1);
	int xDifference(0);
	int y = lastY + direc;
	for (int i(1); i < MAP_WIDTH; ++i)
	{
		if (xDistance - xDifference <= 0)
		{
			xDifference = (rand() % X_RANGE) + 1;
			y = lastY + direc;
			xDifference = 0;
		}
		//int y = lastY + direc;

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
		++xDifference;
	}

	for (size_t i(1); i < MAP_WIDTH - 1; ++i) //Redundant to check the first and last tile of x 
	{
		for (size_t j(1); j < MAP_HEIGHT - 1; ++j) //Redundant to check first and last tile of y
		{

			if (blockedMap_[i - 1][j] == FREE_TILE && blockedMap_[i][j] == BLOCKED_TILE && blockedMap_[i + 1][j] == FREE_TILE)
				blockedMap_[i][j] = FREE_TILE;
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
				tile[0].color = sf::Color::White;
				tile[1].color = sf::Color::White;
				tile[2].color = sf::Color::White;
				tile[3].color = sf::Color::White;
				if (blockedMap_[i][j - 1] != BLOCKED_TILE)//if tile above is free (top of terrain)
				{
					tile[0].texCoords = sf::Vector2f(0, 0);
					tile[1].texCoords = sf::Vector2f(64, 0);
					tile[2].texCoords = sf::Vector2f(64, 64);
					tile[3].texCoords = sf::Vector2f(0, 64);
				}
				else if (blockedMap_[i][j + 1] != BLOCKED_TILE)//if the tile below if free (bottom of terrain)
				{
					tile[0].texCoords = sf::Vector2f(128, 0);
					tile[1].texCoords = sf::Vector2f(192, 0);
					tile[2].texCoords = sf::Vector2f(192, 64);
					tile[3].texCoords = sf::Vector2f(128, 64);
				}
				else
				{
					tile[0].texCoords = sf::Vector2f(64, 0);
					tile[1].texCoords = sf::Vector2f(128, 0);
					tile[2].texCoords = sf::Vector2f(128, 64);
					tile[3].texCoords = sf::Vector2f(64, 64);
				}
				break;
			case FREE_TILE:
				tile[0].color = sf::Color::Transparent;
				tile[1].color = sf::Color::Transparent;
				tile[2].color = sf::Color::Transparent;
				tile[3].color = sf::Color::Transparent;
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
