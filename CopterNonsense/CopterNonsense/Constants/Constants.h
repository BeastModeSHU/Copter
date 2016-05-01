#pragma once 

#include <SFML\Graphics.hpp> 

namespace GameConstants
{
	namespace Window
	{
		const size_t WINDOW_WIDTH(1280);
		const size_t WINDOW_HEIGHT(720);
		const sf::String WINDOW_TITLE("Copter Nonsense");
		const static sf::ContextSettings CONTEXT_SETTINGS(0, 0, 2, 1, 1, 0);
	}

	namespace Gameplay
	{
		const size_t MAX_BULLETS(5);
		const size_t MAX_ANTIGRAV(10);
	}

	namespace Map
	{
		const size_t MAP_WIDTH(250);
		const size_t MAP_HEIGHT(12);
		const size_t TILESIZE(64);
	}
}