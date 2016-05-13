#pragma once

#include "..\GameObject\GameObject.h"

class Player
{
public:
	Player();
	~Player();

	//Mutators
	bool initialise();
	void setGameObject(GameObject* obj) { p_object_ = obj; }
	void update(float dtime, const sf::Vector3f&, const int, const bool);
	void applyForce(sf::Vector2f force) { force_ += force; }
	void setGravity(sf::Vector2f g) { gravity_ = g; }
	void events(const sf::Event&);
	void setPosition(sf::Vector2f);
	void resetForce() { force_ = sf::Vector2f(0.f, 0.f); }
	void resetVelocity() { velocity_.x = START_HORIZONTAL_SPEED; }
	//Accessors
	sf::Vector2f getPosition() const { return p_object_->getPosition(); };
	sf::FloatRect getGlobalBounds() const;//{ return p_object_->getGlobalBounds(); }
	sf::Vector2f getVelocity() const { return velocity_; }

private:
	void stepForces();
	void stepVelocity(float);
private:
	const float MAX_HORIZONTAL_SPEED = 700.f;
	const float START_HORIZONTAL_SPEED = 250.f;
	GameObject* p_object_;
	sf::Vector2f force_;
	sf::Vector2f velocity_;
	sf::Vector2f gravity_;
	sf::Texture texture_;
	sf::Texture reverseTexture_;
	float mass_;
	float invMass_;
	bool verticalAllowed_; //Can the vehicle move vertically (for input detection)
};

