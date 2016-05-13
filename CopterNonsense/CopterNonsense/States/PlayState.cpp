#include "PlayState.h"


PlayState::PlayState(sf::RenderWindow* rWin, sf::RenderTexture* rTex)
	: State(Play, rWin, rTex)
{
	srand(time(NULL));
	p_texMngr_ = TextureManager::instance();
}

PlayState::~PlayState()
{
	delete p_texMngr_;

	for (GameObject* obj : p_objects_)
		delete obj;
}

//Public functions

bool PlayState::init()
{
	startGame_ = false;

	antiGravs_.resize(GameConstants::Gameplay::MAX_ANTIGRAV);
	for (int i(0); i < GameConstants::Gameplay::MAX_ANTIGRAV; ++i)
	{
		p_objects_.push_back(new GameObject());
		antiGravs_[i].setGameObject(p_objects_.back());
		antiGravs_[i].initialise();
	}
	setAntiGravPositions();


	map_.generateMap();

	p_objects_.push_back(new GameObject());
	player_.setGameObject(p_objects_.back());

	player_.initialise();
	player_.setPosition(map_.getPlayerStartLocation());

	bullets_.resize(GameConstants::Gameplay::MAX_BULLETS);

	for (int i(0); i < GameConstants::Gameplay::MAX_BULLETS; ++i)
	{
		p_objects_.push_back(new GameObject());
		bullets_[i].setGameObject(p_objects_.back());
		bullets_[i].initialise();
	}

	obstacles_.resize(GameConstants::Gameplay::MAX_OBSTACLES);
	for (int i(0); i < GameConstants::Gameplay::MAX_OBSTACLES; ++i)
	{
		p_objects_.push_back(new GameObject());
		obstacles_[i].setGameObject(p_objects_.back());
		obstacles_[i].initialise();
	}

	setObstaclePositions();

	initViewPosition();

	gameplayState_ = Playing;

	if (!font_.loadFromFile("res//fonts//Seriphim.ttf"))
		return(false);
	if (!backgroundTexture_.loadFromFile("res//background.jpg"))
		return(false);
	backgroundTexture_.setRepeated(true);

	background_.setSize(sf::Vector2f(GameConstants::Map::MAP_WIDTH * GameConstants::Map::TILESIZE * 2, GameConstants::Window::WINDOW_HEIGHT));
	background_.setScale(1, 1);
	background_.setPosition(-400, 0);
	background_.setTexture(&backgroundTexture_);
	background_.setTextureRect(sf::IntRect(0, 0, background_.getSize().x, background_.getSize().y));

	pauseText_.setFont(font_);
	pauseText_.setCharacterSize(72);
	pauseText_.setString("Game Paused!");
	pauseText_.setColor(sf::Color::White);

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

	score_ = 0;
	highscore_ = 0;

	return(true);
}

void PlayState::draw() const
{
	p_rtexture_->draw(background_);

	for (GameObject* obj : p_objects_)
		if (obj->getAlive())
			p_rtexture_->draw(*obj);

	p_rtexture_->draw(map_);
	p_rtexture_->draw(scoreText_);

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
	if (!startGame_)
	{
		player_.update(delta, map_.currentCol_, gravity_, startGame_);
		map_.lerpColours(delta, player_.getPosition());
		for (int i(0); i < GameConstants::Gameplay::MAX_ANTIGRAV; ++i)
		{
			antiGravs_[i].update(map_.currentCol_);
		}
	}
	else if (startGame_)
	{
		scoreText_.setString(" SCORE : " + std::to_string((int)score_));
		highscoreText_.setString("Current Highscore : " + std::to_string((int)highscore_));
		background_.move(-player_.getVelocity().x * 0.0001, 0);
		sf::Vector2i mousePos = sf::Mouse::getPosition(*p_window_);
		mouseWorldPos_ = p_rtexture_->mapPixelToCoords(mousePos);
		map_.lerpColours(delta, player_.getPosition());

		for (int i(0); i < GameConstants::Gameplay::MAX_ANTIGRAV; ++i)
			antiGravs_[i].update(map_.currentCol_);
		for (int i(0); i < GameConstants::Gameplay::MAX_OBSTACLES; ++i)
			obstacles_[i].update(map_.currentCol_);

		switch (gameplayState_)
		{
		case Playing:
			updatePlaying(delta);
			break;
		case Paused:
			updatePaused(delta);
			break;
		case DeathScreen:
			updateDeathScreen(delta);
			break;
		case WinScreen:
			updateWinScreen(delta);
			break;
		}
	}
}

void PlayState::handleEvent(const sf::Event& evnt)
{
	player_.events(evnt);

	if (evnt.type == sf::Event::KeyPressed)
	{
		if (evnt.key.code == sf::Keyboard::P) //|| evnt.key.code == sf::Keyboard::Escape)
		{
			//pause or unpause the game as long as we're not on the death screen
			gameplayState_ == Paused  && gameplayState_ != DeathScreen ? gameplayState_ = Playing : gameplayState_ = Paused;
		}
	}

	//To be moved to function calls if they grow too large
	switch (gameplayState_)
	{
	case Playing:
	{
		if (evnt.type == sf::Event::KeyPressed)
		{
			if (evnt.key.code == sf::Keyboard::Space)
			{
				if (!startGame_)
					startGame_ = true;
			}
		}
	}
	break;

	case Paused:
	{

	}
	break;

	case DeathScreen:
	{
		if (evnt.type == sf::Event::KeyPressed)
		{
			if (evnt.key.code == sf::Keyboard::R)
			{
				resetGame();
				gameplayState_ = Playing;
			}
		}
	}
	break;
	case WinScreen:
	{
		if (evnt.type == sf::Event::KeyPressed)
		{
			if (evnt.key.code == sf::Keyboard::R)
			{
				resetGame();
				gameplayState_ = Playing;
			}
		}
	}
	break;
	}
}

void PlayState::handleInput(float delta)
{
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
	score_ += delta * 200;
	scoreText_.move(player_.getVelocity().x * delta, 0);
	if (score_ > highscore_)
		highscore_ = score_;
	highscoreText_.move(player_.getVelocity().x * delta, 0);

	bool found(false);
	for (int i(0); i < GameConstants::Gameplay::MAX_ANTIGRAV; ++i)
	{
		if (player_.getGlobalBounds().intersects(antiGravs_[i].getGlobalBounds()))
		{
			found = true;
		}
	}
	if (found)
	{
		gravity_ = -1;
	}
	else
	{
		gravity_ = 1;
	}
	player_.update(delta, map_.currentCol_, gravity_, startGame_);
	translateView(delta);

	if (map_.isTerrainCollision(player_.getGlobalBounds()))
	{
		gameplayState_ = DeathScreen;
	}
	if (player_.getPosition().x > GameConstants::Map::MAP_WIDTH * GameConstants::Map::TILESIZE)
	{
		gameplayState_ = WinScreen;
	}
	//check for collisions with obstacles
	for (int i(0); i < obstacles_.size(); ++i)
	{
		obstacles_[i].update(map_.currentCol_);
		if (obstacles_[i].getGlobalBounds().intersects(player_.getGlobalBounds()))
			gameplayState_ = DeathScreen;
	}
}

void PlayState::updatePaused(float delta)
{
	sf::Vector2f centre(p_rtexture_->getView().getCenter());
	centre -= sf::Vector2f(pauseText_.getGlobalBounds().width / 2.f, pauseText_.getGlobalBounds().height / 2.f);
	pauseText_.setPosition(centre);

}

void PlayState::updateDeathScreen(float delta)
{
	sf::Vector2f centre(p_rtexture_->getView().getCenter());
	centre -= sf::Vector2f(deathText_.getGlobalBounds().width / 2.f, deathText_.getGlobalBounds().height / 2.f);
	deathText_.setPosition(centre);
	scoreText_.setString("Your score was : " + std::to_string((int)score_));
	scoreText_.setPosition(centre.x + 20, centre.y + 150);
}
void PlayState::updateWinScreen(float delta)
{
	sf::Vector2f centre(p_rtexture_->getView().getCenter());
	centre -= sf::Vector2f(winText_.getGlobalBounds().width / 2.f, winText_.getGlobalBounds().height / 2.f);
	winText_.setPosition(centre);
	scoreText_.setString("Your score was : " + std::to_string((int)score_));
	scoreText_.setPosition(centre.x + 20, centre.y + 150);
}
//end of update

//rendering related functions 
void PlayState::drawPauseScreen() const
{
	p_rtexture_->draw(pauseText_);
}

void PlayState::drawDeathScreen() const
{
	p_rtexture_->draw(deathText_);
}
void PlayState::drawWinScreen() const
{
	p_rtexture_->draw(winText_);
}
//end of rendering


//View translation (Camera movement)
void PlayState::initViewPosition()
{
	sf::View view(p_rtexture_->getView());
	float centreX(((player_.getPosition().x) + 400) + player_.getGlobalBounds().width / 2.f);
	//float centreY(player_.getPosition().y);

	//view.setCenter(centreX, centreY);
	view.setCenter(centreX, GameConstants::Window::WINDOW_HEIGHT / 2);

	p_rtexture_->setView(view);
}

void PlayState::translateView(float delta)
{
	sf::View view(p_rtexture_->getView());

	view.move(player_.getVelocity().x * delta, 0.f);

	p_rtexture_->setView(view);
}

void PlayState::setAntiGravPositions()
{
	int startX((rand() % 500) + 500);
	int xJump((rand() % 750) + 4000);
	for (int i(0); i < antiGravs_.size(); ++i)
	{
		antiGravs_[i].setPosition(sf::Vector2f(startX + (xJump * i), 0));
		xJump = rand() % 750 + 2000;
	}
}

void PlayState::setObstaclePositions()
{
	int startX((rand() % 500) + 1000);
	int xJump((rand() % 750) + 1500);
	int yPos(0);
	int xPos(0);
	for (int i(0); i < obstacles_.size(); ++i)
	{
		yPos = rand() % 400 + 100;
		xPos = startX + (xJump * i);
		obstacles_[i].setPosition(sf::Vector2f(xPos, yPos));
		//while (map_.isTerrainCollision(obstacles_[i].getGlobalBounds()))
		//{
		//	yPos = rand() % 400 + 100;
		//	obstacle_.setPosition(xPos, yPos);
		//}
		xJump = (rand() % 750) + 1500;
	}
}

//reset game for replay

void PlayState::resetGame()
{
	map_.generateMap();
	startGame_ = false;
	gravity_ = 1;
	player_.resetForce();
	player_.resetVelocity();
	player_.setPosition(map_.getPlayerStartLocation());
	setAntiGravPositions();
	setObstaclePositions();
	scoreText_.setPosition(player_.getPosition().x, 0);
	score_ = 0;
	//sf::View v(p_rtexture_->getView());
	//v.setCenter(player_.getPosition().x - player_.getGlobalBounds().width / 2.f, v.getCenter().y);
	//p_rtexture_->setView(v);
	initViewPosition();

}
