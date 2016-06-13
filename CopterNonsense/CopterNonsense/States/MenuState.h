#pragma once

#include <iostream> 
#include "State.h"

#include "..\Constants\Constants.h"

using namespace GameConstants::Window;
using namespace GameConstants::Map;

class MenuState :
	public State
{
public:
	MenuState(sf::RenderWindow*, sf::RenderTexture*);
	~MenuState();
	bool init() override;
	void draw() const override;
	void update(float delta) override;
	void handleEvent(const sf::Event& evnt) override;
	void handleInput(float delta) override;
	int nextStateID() const override { return Play; }

private:
	sf::Texture backgroundTexture_;
	sf::RectangleShape backgroundRect_;

	sf::Font font_;
	sf::Text headerText_;
	sf::Text subText_;

	enum States { MAIN, HELP, CREDITS } state;
private:
};