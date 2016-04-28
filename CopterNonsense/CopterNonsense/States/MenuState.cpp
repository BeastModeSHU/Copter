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
	return true;
}

void MenuState::draw() const
{

}

void MenuState::update(float delta)
{

}

void MenuState::handleEvent(const sf::Event & evnt)
{

}

void MenuState::handleInput(float delta)
{

}
