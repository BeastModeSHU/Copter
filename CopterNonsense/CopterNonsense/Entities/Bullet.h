#pragma once

#include "..\GameObject\GameObject.h"

class Bullet
{
public:
	Bullet();
	~Bullet()
	{
		
	};

	bool initialise();
	void setup(const sf::Vector2f&, const sf::Vector2f&);
	void setGameObject(GameObject* obj) { p_object_ = obj; }
	void update(float dtime);
	void applyForce(sf::Vector2f force) { force_ += force; }
	void setGravity(sf::Vector2f g) { gravity_ = g; }
	void setPosition(sf::Vector2f);
	void resetForce() { force_ = sf::Vector2f(0.f, 0.f); }
	void setAlive(bool b) { p_object_->setAlive(b); }
	//Accessors
	bool getAlive() const { return p_object_->getAlive(); }
	sf::Vector2f getPosition() const { return p_object_->getPosition(); };
	sf::FloatRect getGlobalBounds() const { return p_object_->getGlobalBounds(); }
	sf::Vector2f getVelocity() const { return velocity_; }

private:
	void stepForces();
	void stepVelocity(float);
private:
	const float MAX_HORIZONTAL_SPEED = 500.f;
	const float START_HORIZONTAL_SPEED = 100.f;
	float elapsedTime_;
	GameObject* p_object_;
	sf::Vector2f force_;
	sf::Vector2f velocity_;
	sf::Vector2f gravity_;
	sf::Texture texture_;
	float mass_;
	float invMass_;
};