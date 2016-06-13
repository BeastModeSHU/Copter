#include "AntiGrav.h"

bool AntiGrav::initialise()
{
	if (!texture_.loadFromFile("res//antigrav.png")) //Try and load the texture, if it fails return false
		return(false);
	texture_.setRepeated(true); //Repeat the texture across the object

	int sx((rand() % 1000) + 400); //Get a random value for its x-scale
	p_object_->setScale(sx, 720);  //Set the scale of the object
	p_object_->setPosition(500, 0); //Set its position
	p_object_->setAlive(true); 
	p_object_->setTexture(&texture_); //Set the texture of the object
	p_object_->setTextureRect(sf::FloatRect(0, 0, p_object_->getScale().x, p_object_->getScale().y));

	return(true);
}

void AntiGrav::update(const sf::Vector3f& col)
{
	p_object_->setFillColor(sf::Color(col.x, col.y, col.z)); //Change the colour of this object to the colour of the map (Which is being lerped)
}

sf::FloatRect AntiGrav::getGlobalBounds() const
{
	sf::FloatRect bounds;
	bounds.width = p_object_->getScale().x;
	bounds.height = p_object_->getScale().y;
	bounds.left = (p_object_->getPosition().x + (p_object_->getGlobalBounds().width / 2.f)) - bounds.width / 2.f;
	bounds.top = (p_object_->getPosition().y + (p_object_->getGlobalBounds().height / 2.f)) - bounds.height / 2.f;
	return(bounds);
}
