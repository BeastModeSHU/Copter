#pragma once 

#include <SFML\Graphics.hpp> 
//Constant variables to be used across the system
namespace GameConstants
{
	//Window constants
	namespace Window
	{
		const size_t WINDOW_WIDTH(1280);
		const size_t WINDOW_HEIGHT(720);
		const sf::String WINDOW_TITLE("Copter Nonsense");
		const static sf::ContextSettings CONTEXT_SETTINGS(0, 0, 2, 1, 1, 0);
	}
	
	//Gameplay constants
	namespace Gameplay
	{
		const size_t MAX_BULLETS(5);
		const size_t MAX_ANTIGRAV(30);
		const size_t MAX_OBSTACLES(30);
		const size_t SCORE_MULTIPLIER(200);
	}
	
	//Map constants
	namespace Map
	{
		const size_t MAP_WIDTH(400);
		const size_t MAP_HEIGHT(12);
		const size_t TILESIZE(64);
	}
}