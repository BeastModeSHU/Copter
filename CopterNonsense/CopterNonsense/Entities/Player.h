#pragma once

#include "..\GameObject\GameObject.h"

//Object to hold all the data for the player gameobject and to handle the interaction between the player and player object
//Written by Masimba Walker & Joe Thomas

class Player
{
public:
	Player();
	~Player() = default;

	//Mutators
	bool initialise(); //Initalise the object (Texture etc.)
	void setGameObject(GameObject* obj) { p_object_ = obj; } //Set the game object pointer up
	void update(float dtime, const sf::Vector3f&, const int, const bool); //Update the object (Called every frame)
	void applyForce(sf::Vector2f force) { force_ += force; } //Apply a force to the player body
	void setGravity(sf::Vector2f g) { gravity_ = g; } //Set the gravity being applied to the player
	void events(const sf::Event&); //Handle the events (Keyboard presses etc.)
	void setPosition(sf::Vector2f p) { p_object_->setPosition(p); } //Set the position of the player
	void resetForce() { force_ = sf::Vector2f(0.f, 0.f); } //Reset the forces being applied to the player
	void resetVelocity() { velocity_.x = START_HORIZONTAL_SPEED; } //Reset the velocity of the player
	//Accessors
	sf::Vector2f getPosition() const { return p_object_->getPosition(); }; //Get the position of the player
	sf::FloatRect getGlobalBounds() const; //Get the global bounds of the player (Used for AABB collision)
	sf::Vector2f getVelocity() const { return velocity_; } //Get the current velocity of the player

private:
	void stepForces(); //Update the forces being applied to the player
	void stepVelocity(float); //Update the velocity of the player
private:
	const float MAX_HORIZONTAL_SPEED = 700.f; //Constant high/max speed value
	const float START_HORIZONTAL_SPEED = 250.f; //Constant start speed value
	GameObject* p_object_; //Pointer to the game object
	sf::Vector2f force_; //The current force being acted on the player
	sf::Vector2f velocity_; //The current velocity of the player
	sf::Vector2f gravity_; //The current value of gravity being acted on the player
	sf::Texture texture_; //The texture for the object
	sf::Texture reverseTexture_; //The texture flipped (For when you go upside down in anti gravity - Lazy way of doing it)
	float mass_; //The mass of the player (Effects the forces being acted on the player)
	float invMass_; //1/Mass (Pre calculated as it is used a lot) 
	bool verticalAllowed_; //Can the vehicle move vertically (for input detection)
};

