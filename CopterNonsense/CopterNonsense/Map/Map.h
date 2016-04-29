#pragma once 

#include <SFML\Graphics.hpp>

#include "..\Constants\Constants.h"

#define BLOCKED_TILE 1 
#define FREE_TILE 0

class Map :
	public sf::Drawable, public sf::Transformable
{
public:
	Map();
	~Map();

	//Mutators
	void generateMap();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Accessors
	bool isTerrainCollision(const sf::FloatRect& collider)const;
	sf::Vector2f getPlayerStartLocation() const { return playerStart_; }
private: //functions 
	void setupBlockedMap();
	inline void fillMapBlocked();
	void generateRandomBlockedMap();
	void initVertArray();
	int getMinY(int) const;
	int getMaxY(int) const;
	bool isCollidingWithMap(const sf::FloatRect& colider) const;
private:
	const int DIREC_SIZE = 8;
	sf::VertexArray vertexArray_;
	sf::Texture* p_tileset_;
	sf::Texture tileset_;
	int blockedMap_[GameConstants::Map::MAP_WIDTH][GameConstants::Map::MAP_HEIGHT];
	static sf::Vector2i DIRECTIONS[8];
	sf::Vector2f playerStart_;
	/* enum Directions {
		UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT
		};*/

};
