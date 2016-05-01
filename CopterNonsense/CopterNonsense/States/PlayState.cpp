#include "PlayState.h"


PlayState::PlayState(sf::RenderWindow* rWin, sf::RenderTexture* rTex)
	: State(Play, rWin, rTex)
{
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

	initViewPosition();

	gameplayState_ = Playing;

	if (!font_.loadFromFile("res//fonts//Seriphim.ttf"))
		return(false);


	pauseText_.setFont(font_);
	pauseText_.setCharacterSize(72);
	pauseText_.setString("Game Paused!");
	pauseText_.setColor(sf::Color::White);

	deathText_.setFont(font_);
	deathText_.setCharacterSize(72);
	deathText_.setString("You died! R to retry");
	deathText_.setColor(sf::Color::White);


	//Debug purposes
	/*sf::View v(p_rtexture_->getView());
	v.zoom(10);
	p_rtexture_->setView(v);*/
	//Debug purposes
	return(true);
}

void PlayState::draw() const
{
	for (GameObject* obj : p_objects_)
		if (obj->getAlive())
			p_rtexture_->draw(*obj);

	p_rtexture_->draw(map_);


	switch (gameplayState_)
	{
	case Paused:
		drawPauseScreen();
		break;
	case DeathScreen:
		drawDeathScreen();
		break;
	}
}

void PlayState::update(float delta)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*p_window_);
	mouseWorldPos_ = p_rtexture_->mapPixelToCoords(mousePos);
	for (int i(0); i < GameConstants::Gameplay::MAX_BULLETS; ++i)
	{
		bullets_[i].update(delta);
		if (map_.isTerrainCollision(bullets_[i].getGlobalBounds()) && bullets_[i].getAlive())
		{
			bullets_[i].setAlive(false);
		}
	}


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
	}

}

void PlayState::handleEvent(const sf::Event& evnt)
{
	player_.events(evnt);

	if (evnt.type == sf::Event::KeyPressed)
	{
		if (evnt.key.code == sf::Keyboard::M)
		{
			sf::Vector2f pos(player_.getPosition().x + player_.getGlobalBounds().width / 2, player_.getPosition().y + player_.getGlobalBounds().height / 2);
			sf::Vector2f rot(subtract(mouseWorldPos_, pos));

			bool found(false);
			for (int i(0); i < GameConstants::Gameplay::MAX_BULLETS; ++i)
			{
				if (!bullets_[i].getAlive() && !found)
				{
					bullets_[i].setup(rot, pos);
					found = true;
				}
			}
		}
	}

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

	}
}

void PlayState::handleInput(float delta)
{
	switch (gameplayState_)
	{
	case Playing: break;
	case Paused: break;
	case DeathScreen: break;
	}
}

//Private functions

//update related functions 
void PlayState::updatePlaying(float delta)
{
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
	player_.update(delta,map_.currentCol_, gravity_);
	translateView(delta);
	map_.lerpColours(delta, player_.getPosition());

	if (map_.isTerrainCollision(player_.getGlobalBounds()))
	{
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
	centre -= sf::Vector2f(pauseText_.getGlobalBounds().width / 2.f, pauseText_.getGlobalBounds().height / 2.f);
	deathText_.setPosition(centre);
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
//end of rendering


//View translation (Camera movement)
void PlayState::initViewPosition()
{
	sf::View view(p_rtexture_->getView());
	float centreX(player_.getPosition().x + player_.getGlobalBounds().width / 2.f);
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
	int xJump((rand() % 750) + 2500);
	for (int i(0); i < antiGravs_.size(); ++i)
	{
		antiGravs_[i].setPosition(sf::Vector2f(startX + (xJump * i) , 0));
		xJump = rand() % 500 + 500;
	}
}

//reset game for replay

void PlayState::resetGame()
{
	map_.generateMap();
	player_.resetForce();
	player_.setPosition(map_.getPlayerStartLocation());
	//sf::View v(p_rtexture_->getView());
	//v.setCenter(player_.getPosition().x - player_.getGlobalBounds().width / 2.f, v.getCenter().y);
	//p_rtexture_->setView(v);
	initViewPosition();

}
