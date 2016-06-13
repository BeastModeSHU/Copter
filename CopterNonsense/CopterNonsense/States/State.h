#pragma once 

#include <SFML\Graphics.hpp> 

/*
Pure virtual implementation of the game states.
-Written by: M.Walker
*/
enum StateType
{
	Play, Menu, Credits, Help
};

class State
{
public:
	State(StateType, sf::RenderWindow* const, sf::RenderTexture * const);
	virtual ~State();
	virtual bool init() = 0;
	virtual void draw() const = 0;
	virtual void update(float) = 0;
	virtual void handleEvent(const sf::Event&) = 0;
	virtual void handleInput(float) = 0;
	virtual int nextStateID() const = 0;

	virtual void endState() { completed_ = true; }
	void resetState() { completed_ = false; }

	bool isStateFinished() const { return completed_; }
	StateType getStateType() const { return type_; }

protected:
	//A pointer to Window & render texture required by each state
	sf::RenderWindow*  p_window_; //pointer to the renderwindow 
	sf::RenderTexture*  p_rtexture_; //Pointer to the texture shit will be drawn to 

private:
	StateType type_; //The id of this state
	bool completed_ = false;
};