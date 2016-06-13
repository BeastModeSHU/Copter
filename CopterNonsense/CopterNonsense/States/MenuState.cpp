#include "MenuState.h"

MenuState::MenuState(sf::RenderWindow * rWin, sf::RenderTexture * rTex)
	: State(Menu, rWin, rTex)
{
}

MenuState::~MenuState()
{
}

bool MenuState::init()
{

	state = MAIN;

	if (!backgroundTexture_.loadFromFile("res//background.jpg"))
		return(false);

	backgroundRect_.setTexture(&backgroundTexture_);
	backgroundRect_.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	backgroundRect_.setScale(1, 1);
	backgroundRect_.setPosition(-250, 0);


	if (!font_.loadFromFile("res//fonts//Seriphim.ttf"))
		return(false);

	headerText_.setFont(font_);
	headerText_.setCharacterSize(80);
	headerText_.setString("COPTER NONSENSE");
	headerText_.setPosition(p_rtexture_->mapPixelToCoords(sf::Vector2i(((WINDOW_WIDTH - headerText_.getGlobalBounds().width) / 2), (WINDOW_HEIGHT - headerText_.getGlobalBounds().height) / 2 * 0.25)));

	subText_.setFont(font_);
	subText_.setCharacterSize(50);
	subText_.setString("Press P to Play..\nPress I for Instructions..\nPress C for Credits..\nPress ESC to Quit..");
	subText_.setPosition(p_rtexture_->mapPixelToCoords(sf::Vector2i(((WINDOW_WIDTH - headerText_.getGlobalBounds().width) / 2) * 0.8, (WINDOW_HEIGHT - headerText_.getGlobalBounds().height) / 2 * 0.8)));

	return (true);
}

void MenuState::draw() const
{
	p_rtexture_->draw(headerText_);
	p_rtexture_->draw(subText_);
}

void MenuState::update(float delta)
{
	switch (state)
	{
	case MAIN:
		headerText_.setString("COPTER NONSENSE");
		headerText_.setCharacterSize(80);
		subText_.setCharacterSize(50);
		subText_.setString("Press P to Play..\nPress I for Instructions..\nPress C for Credits..\nPress ESC to Quit..");
		subText_.setPosition(p_rtexture_->mapPixelToCoords(sf::Vector2i(((WINDOW_WIDTH - headerText_.getGlobalBounds().width) / 2) * 0.8, (WINDOW_HEIGHT - headerText_.getGlobalBounds().height) / 2 * 0.8)));
		break;
	case HELP:
		headerText_.setString("INSTRUCTIONS");
		headerText_.setCharacterSize(80);
		subText_.setCharacterSize(50);
		subText_.setString("Press SPACE to Jump..\nWatch out for the anti-gravity fields\nThese are marked by up arrows\nThis flips the gravity so jumping now moves you down\nAlso watch for out for the obstacles!\nYour score increases the further you get\nPress BACKSPACE for main menu...");
		subText_.setPosition(p_rtexture_->mapPixelToCoords(sf::Vector2i(((WINDOW_WIDTH - headerText_.getGlobalBounds().width) / 2) * 0.8, (WINDOW_HEIGHT - headerText_.getGlobalBounds().height) / 2 * 0.8)));
		break;
	case CREDITS:
		headerText_.setString("CREDITS");
		headerText_.setCharacterSize(80);
		subText_.setCharacterSize(50);
		subText_.setString("Programmers...\nAlan Masimba Walker\nJoe Thomas\nPress BACKSPACE for main menu");
		subText_.setPosition(p_rtexture_->mapPixelToCoords(sf::Vector2i(((WINDOW_WIDTH - headerText_.getGlobalBounds().width) / 2) * 0.8, (WINDOW_HEIGHT - headerText_.getGlobalBounds().height) / 2 * 0.8)));
		break;
	}
}

void MenuState::handleEvent(const sf::Event & evnt)
{
	if (evnt.type == sf::Event::KeyPressed)
	{
		if (evnt.key.code == sf::Keyboard::P)
		{
			if (state == MAIN)
				endState();
		}
		if (evnt.key.code == sf::Keyboard::I)
		{
			if (state == MAIN)
				state = HELP;
			else if(state == HELP)
				state = MAIN;
		}
		if (evnt.key.code == sf::Keyboard::C)
		{
			if (state == MAIN)
				state = CREDITS;
			else if (state == CREDITS)
				state = MAIN;
		}
		if (evnt.key.code == sf::Keyboard::BackSpace)
		{
			if (state != MAIN)
				state = MAIN;
		}
	}
}

void MenuState::handleInput(float delta)
{

}
