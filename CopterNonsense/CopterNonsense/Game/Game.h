#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <assert.h>
//Game includes
#include "..\Constants\Constants.h"
#include "..\States\State.h"
#include "..\States\PlayState.h"
#include "..\States\MenuState.h"

/*
Game manager, handles moving between states and the overall initialisation of the game and cleanup of the game.
-Written by: M. Walker
*/
using namespace GameConstants::Window;

class Game
{
public:
	Game();
	~Game();
	bool init();
	void run();
private: //Functions 
	void setFPSText(float delta);
	void setCurrentState(StateType);
	void freeStates();
private:
	std::vector<State*>states_; //List of game states (will be looped through)
	sf::RenderWindow window_; //Window to be displayed to 
	sf::RenderTexture rTexture_; //Render texture to be drawn to 
	sf::Font fpsFont_; //Font for the fps text
	sf::Text fpsText_; //Text to display the fps
	const sf::String FPS_TEXT; //String holding text for fps
	const int moveSpeed; //move speed of the camera

	int quadWidth_;
	int quadHeight_;
	int skippedFrames = 1;
	const int MAX_SKIPPED_FRAMES = 5;
	const sf::Int32 MS_PER_UPDATE = 16; //Maximum amount of time that must elapse before we will update (Set for a 60 FPS game)
	const float targetFps = 60;
	const float deltaTime = 1 / targetFps;
	State* currentState_;
	int stateCounter_;		//keeps the index for which state you are on

	//Map raceMap_;	//Object holding all of the map data
};

