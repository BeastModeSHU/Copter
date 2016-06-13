#pragma once

#include "..\GameObject\GameObject.h"

//Object that is used to block off parts of the map as obstacles to dodge
//Written by Joe Thomas

class Obstacle
{
public:

	Obstacle() = default;
	~Obstacle() = default;
	
	bool initialise(); //Initalise the object, load the texture, set its size etc.
	void setGameObject(GameObject* obj) { p_object_ = obj; } //Set the pointer up for the game object

	void update(const sf::Vector3f&); //Update the object every frame (Change its colour)

	void setPosition(const sf::Vector2f& pos) { p_object_->setPosition(pos); } //Set the position of the object
	sf::Vector2f getPosition() const { return p_object_->getPosition(); };     //Get the position of the object
	sf::FloatRect getGlobalBounds() const; //Get the global bounds of the object (Used for checking collision)

private:
	GameObject* p_object_; //Pointer for the gameobject
	sf::Texture texture_; //Texture for the object
};