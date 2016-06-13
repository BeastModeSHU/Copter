#include "Game.h"


Game::Game()
	: FPS_TEXT("Current FPS : "), moveSpeed(20), stateCounter_(0)
{
}

Game::~Game()
{
	for (State* s : states_)
		delete s;
	//assert(s == nullptr);
}

bool Game::init()
{
	window_.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Close, CONTEXT_SETTINGS); //Create a window 
	states_.push_back(new PlayState(&window_, &rTexture_)); //Push the play state into the list of states
	states_.push_back(new MenuState(&window_, &rTexture_)); //Push the menu state into the list of statesz
	//states_.push_back(new MenuState(&window_, &rTexture_)); //Push the menu state into the list of states

	if (!rTexture_.create(WINDOW_WIDTH, WINDOW_HEIGHT))
		return(false);

	for (State* s : states_)
	{//Loop through all states in the list
		assert(s != nullptr);
		if (!s->init()) //if we failed to init a state
			return(false);//exit out of the game initialise
	}
	//setCurrentState(Play);
	setCurrentState(Menu);
	if (!fpsFont_.loadFromFile("res//fonts//seriphim.ttf"))
		return(false);

	fpsText_.setFont(fpsFont_);
	fpsText_.setCharacterSize(16);
	fpsText_.setColor(sf::Color::White);

	return(true);
}

void Game::run()
{
	//window_.setFramerateLimit(60);
	bool hasFocus(true); //does have window focus
	sf::Clock elapsed; //Used to record the elapsed time of the game 
	sf::Clock delta; //Used to record the time between each update loop


	float lag(0.f);
	sf::Time previous(elapsed.getElapsedTime());

	//While the window is open and running
	while (window_.isOpen())
	{

		float current(elapsed.getElapsedTime().asSeconds());
		float elapsed(current - previous.asSeconds());
		lag += elapsed;
		previous = sf::Time(sf::seconds(elapsed));

		//State* s(states_[currentState]); //Create a pointer to the current state we're in
		assert(currentState_); //assert that the pointer isn't null
		sf::Event evnt; //Make an sfml event 

		float dtime = delta.restart().asSeconds(); //Initialise an object of game time with the elapsed time, and delta time

		while (window_.pollEvent(evnt))//Get all the events that have occurred
		{
			if (evnt.type == sf::Event::Closed)
				window_.close();

			if (evnt.type == sf::Event::LostFocus)
				hasFocus = false;

			if (evnt.type == sf::Event::GainedFocus)
				hasFocus = true;

			if (evnt.type == sf::Event::KeyReleased)
				if (evnt.key.code == sf::Keyboard::Escape)
					window_.close();

			if (window_.isOpen() && hasFocus)
			{//prevents the game from handling events post-close and only run the event handler when the user is in the game
				currentState_->handleEvent(evnt);
			}

		}

		if (hasFocus)
			currentState_->handleInput(dtime);

		if (window_.isOpen())
		{//prevents the window from updating post-closing & only run update when the user is in the game

			if (lag > deltaTime)//clamp the 
				lag = deltaTime;
			//while (lag.asMilliseconds() >= MS_PER_UPDATE)
			//while (lag >= deltaTime)
			{
				currentState_->update(dtime);
				if (currentState_->isStateFinished())
				{
					//++stateCounter_;
					currentState_->resetState();

					stateCounter_ = currentState_->nextStateID();
					currentState_ = states_[stateCounter_];
				}


				if (skippedFrames > MAX_SKIPPED_FRAMES)
				{

					rTexture_.clear(sf::Color::Black); //Clear the render texture with a colour of black

					currentState_->draw();
					rTexture_.display();

					//calculateFPS(time);

					const sf::Texture& rendered_texture(rTexture_.getTexture());
					const sf::Sprite sprite(rendered_texture);

					window_.clear(sf::Color::Black);
					window_.draw(sprite);
					window_.draw(fpsText_);
					window_.display();
					skippedFrames = 1;
				}
				else
				{
					++skippedFrames;
				}


				lag -= deltaTime;

			}

			rTexture_.clear(sf::Color::Black); //Clear the render texture with a colour of black

			currentState_->draw();
			rTexture_.display();

			setFPSText(dtime);

			const sf::Texture& rendered_texture(rTexture_.getTexture());
			const sf::Sprite sprite(rendered_texture);

			window_.clear(sf::Color::Black);
			window_.draw(sprite);
			window_.draw(fpsText_);
			window_.display();
		}
	}
}

//Private functions 

//Function to calculate the fps of the game and set the fps counter at the top left to a different number
void Game::setFPSText(float delta)
{
	fpsText_.setString(FPS_TEXT + std::to_string(static_cast<int> (1 / delta)));
	sf::View s(window_.getView());
	//Align the fps counter at the top left
	float xPos = (s.getCenter().x - (s.getSize().x / 2.f)) + (fpsText_.getCharacterSize() / 2.f);
	float yPos = (s.getCenter().y - (s.getSize().y / 2.f)) + (fpsText_.getCharacterSize() / 2.f);
	fpsText_.setPosition(xPos, yPos);
}

void Game::setCurrentState(StateType type)
{
	int i(0);
	bool found(false);
	while (!found && i < states_.size())
	{

		if (states_[i]->getStateType() == type)

		{
			currentState_ = states_[i];
			found = true;
		}
		++i;
	}
	if (!found)
		currentState_ = nullptr;
}
