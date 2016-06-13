#include "PlayState.h"


PlayState::PlayState(sf::RenderWindow* rWin, sf::RenderTexture* rTex)
	: State(Play, rWin, rTex)
{
	srand(time(NULL)); //Set the seed for rng
	p_texMngr_ = TextureManager::instance(); //Get an instance of the texture manager (Singleton)
}

PlayState::~PlayState()
{
	//Delete pointers (Stop memory leaks)
	delete p_texMngr_;
	for (GameObject* obj : p_objects_)
		delete obj;
}

//Public functions

bool PlayState::init()
{
	startGame_ = false; //hold the game at the start

	//--Antigrav Initialisation--
	antiGravs_.resize(GameConstants::Gameplay::MAX_ANTIGRAV); //resize the antigrav vector 
	for (int i(0); i < GameConstants::Gameplay::MAX_ANTIGRAV; ++i)
	{
		p_objects_.push_back(new GameObject()); //push new gameobjects for each antigrav
		antiGravs_[i].setGameObject(p_objects_.back()); //set the pointer to the new gameobject so it can be accessed through the antigrav object
		antiGravs_[i].initialise(); //initialise the antigrav object
	}
	setAntiGravPositions(); //setup the positions of each antigrav

	//--Map Initialisation--
	map_.generateMap(); //generate a new map (random)

	//--Player Initialisation--
	p_objects_.push_back(new GameObject()); //push a new gameobject on for the player
	player_.setGameObject(p_objects_.back()); //set the pointer to the gameobject
	player_.initialise(); //initialise the player
	player_.setPosition(map_.getPlayerStartLocation()); //set the position of the player

	//--Obstacle Initialisation
	obstacles_.resize(GameConstants::Gameplay::MAX_OBSTACLES); //resize obstacle vector
	for (int i(0); i < GameConstants::Gameplay::MAX_OBSTACLES; ++i)
	{
		p_objects_.push_back(new GameObject()); //push a new gameobject for each obstacle
		obstacles_[i].setGameObject(p_objects_.back()); //set the pointer to the gameobject
		obstacles_[i].initialise(); //initialise each obstacle
	}
	setObstaclePositions(); //setup each obstacles position

	initViewPosition(); //Initialise the position of the view

	gameplayState_ = Playing; //Set the gameplay state to playing

	//--Texture loading
	if (!font_.loadFromFile("res//fonts//Seriphim.ttf")) //Load in the font
		return(false);
	if (!backgroundTexture_.loadFromFile("res//background.jpg")) //Load in the background texture
		return(false);

	//--Background Initialisation
	backgroundTexture_.setRepeated(true); //Set the background texture to repeat (Tile)
	background_.setSize(sf::Vector2f(GameConstants::Map::MAP_WIDTH * GameConstants::Map::TILESIZE * 2, GameConstants::Window::WINDOW_HEIGHT)); //Set the size of the background texture
	background_.setScale(1, 1); //Set its scale (not needed)
	background_.setPosition(-400, 0); //Set its initial position
	background_.setTexture(&backgroundTexture_); //Set the texture of the background rectangle shape
	background_.setTextureRect(sf::IntRect(0, 0, background_.getSize().x, background_.getSize().y)); //Set its texture rect (Default)

	//--Text Initialistion
	pauseText_.setFont(font_); //Set the font 
	pauseText_.setCharacterSize(72); //Set the size of the font
	pauseText_.setString("Game Paused!"); //Set the initial string 
	pauseText_.setColor(sf::Color::White); //Set the color

	deathText_.setFont(font_);
	deathText_.setCharacterSize(72);
	deathText_.setString("You died! R to retry");
	deathText_.setColor(sf::Color::White);

	winText_.setFont(font_);
	winText_.setCharacterSize(72);
	winText_.setString("WELL DONE M9!");
	winText_.setColor(sf::Color::White);

	scoreText_.setFont(font_);
	scoreText_.setCharacterSize(42);
	scoreText_.setString(" SCORE : ");
	scoreText_.setColor(sf::Color::White);
	scoreText_.setPosition(player_.getPosition().x, 0);

	highscoreText_.setFont(font_);
	highscoreText_.setCharacterSize(28);
	highscoreText_.setString("Current Highscore : " + std::to_string((int)highscore_));
	highscoreText_.setColor(sf::Color::White);
	highscoreText_.setPosition(p_rtexture_->mapPixelToCoords(sf::Vector2i(200, 0)));

	score_ = 0; //Set score to 0
	highscore_ = 0; //Set highscore to 0

	return(true);
}

void PlayState::draw() const
{
	p_rtexture_->draw(background_); //Draw the background rectangle

	//Loop through the p_objects_ vector and draw each object (Player, antigravs, obstacles)
	for (GameObject* obj : p_objects_)
		if (obj->getAlive())
			p_rtexture_->draw(*obj);

	p_rtexture_->draw(map_); //Draw the map
	p_rtexture_->draw(scoreText_); //Draw the score text

	//Based on the state of gameplay, draw different things (UI elements)
	switch (gameplayState_)
	{
	case Paused:
		drawPauseScreen();
		break;
	case DeathScreen:
		drawDeathScreen();
		break;
	case WinScreen:
		drawWinScreen();
		break;
	}
}

void PlayState::update(float delta)
{
	//If the game hasn't started
	if (!startGame_)
	{
		map_.lerpColours(delta, player_.getPosition());	//Lerp the colour of the map
		player_.update(delta, map_.currentCol_, gravity_, startGame_); //Lerp the colour of the player
		for (int i(0); i < GameConstants::Gameplay::MAX_ANTIGRAV; ++i) //Loop through each antigrav
		{
			antiGravs_[i].update(map_.currentCol_); //Lerp the colour of antigravs
		}
	}
	//If the game has started
	else if (startGame_)
	{
		scoreText_.setString(" SCORE : " + std::to_string((int)score_)); //Update the score text to show the current score
		highscoreText_.setString("Current Highscore : " + std::to_string((int)highscore_)); //Update the highscore text to show the current highscore
		background_.move(-player_.getVelocity().x * 0.0001, 0); //move the background in the opposite direction of the player at a percentage of the players velocity
		sf::Vector2i mousePos = sf::Mouse::getPosition(*p_window_); //Get the mouse position in pixels (Where it is in the window)
		mouseWorldPos_ = p_rtexture_->mapPixelToCoords(mousePos); //Get the world position of the mouse and set it into the global vector

		//--Colour Lerping
		map_.lerpColours(delta, player_.getPosition()); //Map colours
		for (int i(0); i < GameConstants::Gameplay::MAX_ANTIGRAV; ++i)
			antiGravs_[i].update(map_.currentCol_); //Antigrav colours
		for (int i(0); i < GameConstants::Gameplay::MAX_OBSTACLES; ++i)
			obstacles_[i].update(map_.currentCol_); //Obstacle colours

		//Depending on the state of gameplay, update different things
		switch (gameplayState_)
		{
		case Playing:
			updatePlaying(delta); //Update if the game is playing
			break;
		case Paused:
			updatePaused(delta); //Update if the player has paused the game
			break;
		case DeathScreen:
			updateDeathScreen(delta); //Update if the player has died
			break;
		case WinScreen:
			updateWinScreen(delta); //Update if the player has won (Got to the end of the track)
			break;
		}
	}
}

void PlayState::handleEvent(const sf::Event& evnt)
{
	player_.events(evnt); //Events function for the player (Handles player movement)

	if (evnt.type == sf::Event::KeyPressed)
	{
		if (evnt.key.code == sf::Keyboard::P) //|| evnt.key.code == sf::Keyboard::Escape)
		{
			//pause or unpause the game as long as we're not on the death screen
			if (gameplayState_ != DeathScreen)
			{
				if (gameplayState_ == Paused)
					gameplayState_ = Playing;
				else
					gameplayState_ = Paused;
			}
		}
	}

	//To be moved to function calls if they grow too large
	switch (gameplayState_)
	{
	case Playing:
	{
		if (evnt.type == sf::Event::KeyPressed)
			if (evnt.key.code == sf::Keyboard::Space)
				if (!startGame_)
					startGame_ = true; //Start the game when player hits spacebar at the start
	}
	break;

	case Paused:
	{
		if (evnt.type == sf::Event::KeyPressed)
			if (evnt.key.code == sf::Keyboard::Q)
				endState(); //End the game if the player pressed Q when paused
	}
	break;

	case DeathScreen:
	case WinScreen:
	{
		if (evnt.type == sf::Event::KeyPressed)
		{
			if (evnt.key.code == sf::Keyboard::R)
			{
				resetGame(); //Reset the game once the player has died or won
				gameplayState_ = Playing;
			}
		}
	}
	break;
	}
}

void PlayState::handleInput(float delta)
{
	//Nothing being done here as of yet
	switch (gameplayState_)
	{
	case Playing: break;
	case Paused: break;
	case DeathScreen: break;
	case WinScreen: break;
	}
}

//Private functions

//update related functions 
void PlayState::updatePlaying(float delta)
{
	score_ += delta * SCORE_MULTIPLIER; //Increment the score counter by how the game has run multiplied by the multiplier
	scoreText_.move(player_.getVelocity().x * delta, 0); //Move the text along with the player
	if (score_ > highscore_) //If the score is greater than the highscore
		highscore_ = score_; //Change the highscore to equal the score
	highscoreText_.move(player_.getVelocity().x * delta, 0); //Move the highscore text along with the player

	//--Antigrav collision
	bool found(false); //Bool to check only one antigrav is being collided with
	for (int i(0); i < GameConstants::Gameplay::MAX_ANTIGRAV; ++i) //Loop through all antigravs
	{
		if (player_.getGlobalBounds().intersects(antiGravs_[i].getGlobalBounds())) //Check the player is colliding with one
		{
			found = true; //If so then found = true
		}
	}
	if (found) //If found = true then flip the gravity (set to -1)
	{
		gravity_ = -1;
	}
	else //Else set the gravity to normal (Set to 1)
	{
		gravity_ = 1;
	}


	player_.update(delta, map_.currentCol_, gravity_, startGame_); //Update the player
	translateView(delta); //Translate the view

	if (map_.isTerrainCollision(player_.getGlobalBounds())) //Check whether the player has collided with the map
	{
		gameplayState_ = DeathScreen; //If so change the state to death screen
	}
	if (player_.getPosition().x > GameConstants::Map::MAP_WIDTH * GameConstants::Map::TILESIZE) //Check whether the player has reached the end of the track
	{
		gameplayState_ = WinScreen; //If so change the state to win screen
	}

	//check for collisions with obstacles
	for (int i(0); i < obstacles_.size(); ++i)
	{
		obstacles_[i].update(map_.currentCol_); //Update the obstacles (Lerp their colours)
		if (obstacles_[i].getGlobalBounds().intersects(player_.getGlobalBounds())) //Check whether the player has collided with an obstacle
			gameplayState_ = DeathScreen; //If so change the state to death screen
	}
}

void PlayState::updatePaused(float delta)
{
	sf::Vector2f centre(p_rtexture_->getView().getCenter()); //Get the centre of the view
	centre -= sf::Vector2f(pauseText_.getGlobalBounds().width / 2.f, pauseText_.getGlobalBounds().height / 2.f); //Make sure the text is centred onto that point
	pauseText_.setPosition(centre); //Set the paused text to that position

}

void PlayState::updateDeathScreen(float delta)
{
	sf::Vector2f centre(p_rtexture_->getView().getCenter()); //Get the centre of the view
	centre -= sf::Vector2f(deathText_.getGlobalBounds().width / 2.f, deathText_.getGlobalBounds().height / 2.f); //Make sure the text is centred onto that point
	deathText_.setPosition(centre); //Set the death text to that position
	scoreText_.setString("Your score was : " + std::to_string((int)score_)); //Update the score text string to have the most recent score
	scoreText_.setPosition(centre.x + 20, centre.y + 150); //Move the score text just below the death text
}
void PlayState::updateWinScreen(float delta)
{
	sf::Vector2f centre(p_rtexture_->getView().getCenter()); //Get the centre of the view
	centre -= sf::Vector2f(winText_.getGlobalBounds().width / 2.f, winText_.getGlobalBounds().height / 2.f); //Make sure the text isc centred onto that point
	winText_.setPosition(centre); //Set the win text to that position
	scoreText_.setString("Your score was : " + std::to_string((int)score_)); //Update the score text string to have the most recent score
	scoreText_.setPosition(centre.x + 20, centre.y + 150); //Move the score text just below the win text
}
//end of update

//rendering related functions 
void PlayState::drawPauseScreen() const
{
	p_rtexture_->draw(pauseText_); //Draw the paused text
}

void PlayState::drawDeathScreen() const
{
	p_rtexture_->draw(deathText_); //Draw the death text
}
void PlayState::drawWinScreen() const
{
	p_rtexture_->draw(winText_); //Draw the win text
}
//end of rendering


void PlayState::initViewPosition()
{
	//Initialise the view position so the player is on the left of the screen
	sf::View view(p_rtexture_->getView());
	float centreX(((player_.getPosition().x) + 400) + player_.getGlobalBounds().width / 2.f);
	view.setCenter(centreX, GameConstants::Window::WINDOW_HEIGHT / 2);
	p_rtexture_->setView(view);
}

void PlayState::translateView(float delta)
{
	//View translation (Camera movement)
	sf::View view(p_rtexture_->getView());
	view.move(player_.getVelocity().x * delta, 0.f);
	p_rtexture_->setView(view);
}

void PlayState::setAntiGravPositions()
{
	//Randomly set up the antigravity positions
	int startX((rand() % 500) + 500); //Get a random start value between 500 - 1000
	int xJump((rand() % 1000) + 2500); //Get a random jump value between 2500 - 3500
	for (int i(0); i < antiGravs_.size(); ++i)
	{
		antiGravs_[i].setPosition(sf::Vector2f(startX + (xJump * i), 0)); //Set its position equal to the random start position + however many jumps along
		xJump = rand() % 1000 + 2500;
	}
}

void PlayState::setObstaclePositions()
{
	//Randomly set up the obstacle positions
	int startX((rand() % 500) + 1000); //Get a random start value between 1000 - 1500
	int xJump((rand() % 750) + 1500); //Get a random jump value between 1500 - 2250
	int yPos(0); //Random yPosition 
	int xPos(0);
	for (int i(0); i < obstacles_.size(); ++i)
	{
		yPos = rand() % 400 + 100;
		xPos = startX + (xJump * i);
		obstacles_[i].setPosition(sf::Vector2f(xPos, yPos));
		xJump = (rand() % 750) + 1500;
	}
}

//reset game for replay

void PlayState::resetGame()
{
	map_.generateMap(); //regenerate a new map
	startGame_ = false;
	gameplayState_ = Playing; //reset the gameplay state to "playing"
	gravity_ = 1;
	player_.resetForce();
	player_.resetVelocity();
	player_.setPosition(map_.getPlayerStartLocation()); //reset the players position and velocity
	setAntiGravPositions(); //reset antigrav positions 
	setObstaclePositions(); //reset obstacle positions
	scoreText_.setPosition(player_.getPosition().x, 0); //reset score text position
	score_ = 0; //reset score back to 0
	initViewPosition();

}

void PlayState::endState()
{
	State::endState();
	resetGame();
}