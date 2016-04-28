#pragma once

#include <iostream> 
#include "State.h"



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
	int nextStateID() const override { return Menu; }

private:

private:
};