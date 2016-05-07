#pragma once

#include "..\GameObject\GameObject.h"

class Obstacle
{
public:

	Obstacle();
	~Obstacle();
	
	bool initialise();
	void setGameObject(GameObject* obj) { p_object_ = obj; }

	void update(const sf::Vector3f&);

	void setPosition(const sf::Vector2f& pos) { p_object_->setPosition(pos); }
	sf::Vector2f getPosition() const { return p_object_->getPosition(); };
	sf::FloatRect getGlobalBounds() const;// { return p_object_->getGlobalBounds(); }
private:
private:

	GameObject* p_object_;
	sf::Texture texture_;


};