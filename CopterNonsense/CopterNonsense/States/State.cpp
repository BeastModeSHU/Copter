#include "State.h"

State::State(StateType id, sf::RenderWindow* const p_window, sf::RenderTexture * const p_rtexture)
	: type_(id), p_window_(p_window), p_rtexture_(p_rtexture)
{

}

State::~State()
{

}
