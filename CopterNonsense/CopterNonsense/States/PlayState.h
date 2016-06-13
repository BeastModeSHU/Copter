#pragma once 

#include <iostream> 
#include <vector>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <time.h>

#include "State.h"
#include "..\Utils\TextureManager.h"
#include "..\GameObject\GameObject.h"
#include "..\Entities\Player.h"
#include "..\Entities\AntiGrav.h"
#include "..\Entities\Obstacle.h"
#include "..\Map\Map.h"

using namespace std;
using namespace GameConstants::Gameplay;

class PlayState :
	public State
{
public:
	PlayState(sf::RenderWindow*, sf::RenderTexture*);
	~PlayState();
	bool init() override;
	void draw() const override;
	void update(float delta) override;
	void handleEvent(const sf::Event& evnt) override;
	void handleInput(float delta) override;
	int nextStateID() const override { return Menu; }

private:
	void initViewPosition();				//initialise the view position when the game starts
	void translateView(float delta);		//move the view once game has started

	void setAntiGravPositions();			//setup positions for the antigrav fields
	void setObstaclePositions();			//setup positions for the obstacles

	//UPDATE FUNCTIONS
	void updatePlaying(float delta);
	void updatePaused(float delta);
	void updateDeathScreen(float delta);
	void updateWinScreen(float delta);
	
	//DRAW FUNCTIONS
	void drawPauseScreen()const;
	void drawDeathScreen() const;
	void drawWinScreen() const;

	void resetGame();						//reset the game once its finished so it can be played again
	void endState();						//end the state and move on to the next state 
private:
	TextureManager* p_texMngr_;				//pointer to the texture manager
	vector<GameObject*> p_objects_;			//container for pointers to the game objects so they can all be drawn at once
	Player player_;							//Player object
	vector<AntiGrav> antiGravs_;			//Anti gravity objects
	vector<Obstacle> obstacles_;			//Obstacle objects
	Map map_;								//Map object
	sf::Font font_;							//Font being used throughout the game
	sf::Text pauseText_;					//Text objects
	sf::Text deathText_; 
	sf::Text scoreText_;
	sf::Text highscoreText_;
	sf::Text winText_;
	sf::Vector2f mouseWorldPos_;			//Vector to hold the world position of mouse
	sf::RectangleShape background_;			//Shape for the background
	sf::Texture backgroundTexture_;			//Texture for the background
	int gravity_ = 1;						//swaps between 1 or -1 depending on which way gravity is flowing
	float score_;							//Keeps hold of the current score
	float highscore_;						//Keeps hold of the current highscore during that play session
	bool startGame_ = false;				//Bool to hold the start of the game until spacebar is hit
	enum GameplayState
	{
		Playing, DeathScreen, Paused, WinScreen
	};
	GameplayState gameplayState_;			//Gameplay states
};