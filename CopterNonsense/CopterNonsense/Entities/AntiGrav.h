#pragma once

#include "..\GameObject\GameObject.h"

//Object that is used to flip the gravity in game
//Written by Joe Thomas

class AntiGrav
{
public:

	AntiGrav() = default;
	~AntiGrav() = default;

	bool initialise(); //Initialise the objects size and texture
	void setGameObject(GameObject* obj) { p_object_ = obj; } //Set the game object pointer 

	void update(const sf::Vector3f&); //Update the object

	void setPosition(const sf::Vector2f& pos) { p_object_->setPosition(pos); }	//Set the position of the object
	sf::Vector2f getPosition() const { return p_object_->getPosition(); };		//Return the position of the object
	sf::FloatRect getGlobalBounds() const; //Get the global bounds of the object (Used for collision)

private:

	GameObject* p_object_; //Pointer to the game object that is related to this object
	sf::Texture texture_; //Texture for the game object

};
