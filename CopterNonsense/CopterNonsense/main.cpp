//////////////////////////////////
//Library Includes
#include <SFML\Graphics.hpp>
//////////////////////////////////
//Game Includes
#include "Game\Game.h"
//////////////////////////////////
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#define _CRTDBG_MAP_ALLOC

/* 
	Main Function 

	-Team Moxeij - Mace
*/

int main(int argc, char* argv[])
{
	{	
	//code to check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(VALUE);
	}
	
	Game game; 
	
	if (!game.init())
		return(EXIT_FAILURE);
	else
		game.run();

	return (EXIT_SUCCESS);
}