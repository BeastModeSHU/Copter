#include "AntiGrav.h"

AntiGrav::AntiGrav()
{
}

AntiGrav::~AntiGrav()
{
}

bool AntiGrav::initialise()
{
	int sx((rand() % 1000) + 400);
	p_object_->setScale(sx, 720);
	p_object_->setPosition(500, 0);
	p_object_->setAlive(true);
	if (!texture_.loadFromFile("res//antigrav.jpg"))
		return(false);
	texture_.setRepeated(true);
	p_object_->setTexture(&texture_);
	p_object_->setTextureRect(sf::FloatRect(0, 0, p_object_->getScale().x, p_object_->getScale().y));

	return true;
}

sf::FloatRect AntiGrav::getGlobalBounds() const
{
	sf::FloatRect bounds;
	bounds.width = p_object_->getScale().x;
	bounds.height = p_object_->getScale().y;
	bounds.left = (p_object_->getPosition().x + (p_object_->getGlobalBounds().width / 2.f)) - bounds.width / 2.f;
	bounds.top = (p_object_->getPosition().y + (p_object_->getGlobalBounds().height / 2.f)) - bounds.height / 2.f;
	/*bounds.left = (p_object_->getPosition().x - bounds.width / 2.f);
	bounds.top = (p_object_->getPosition().y - bounds.height / 2.f);*/
	return(bounds);
}
