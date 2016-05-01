#pragma once

#include "..\GameObject\GameObject.h"

class AntiGrav
{
public:
	AntiGrav();
	~AntiGrav();

	bool initialise();
	void setGameObject(GameObject* obj) { p_object_ = obj; }

	void setPosition(const sf::Vector2f& pos) { p_object_->setPosition(pos); }
	sf::Vector2f getPosition() const { return p_object_->getPosition(); };
	sf::FloatRect getGlobalBounds() const;// { return p_object_->getGlobalBounds(); }

public:
private:
	GameObject* p_object_;
	sf::Texture texture_;

};
