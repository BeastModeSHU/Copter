#include "Obstacle.h"

bool Obstacle::initialise()
{

	if (!texture_.loadFromFile("res//obstacle.png")) //Try and load the texture, if failed then return false
		return(false);
	texture_.setRepeated(true); //Repeat the texture (Tile it)

	p_object_->setScale(sf::Vector2f(25, (rand() % 100 + 75))); //Set the size of the obstacle
	p_object_->setPosition(0, 0); //Set its initial position (Will be changed)
	p_object_->setAlive(true); //Set the object to alive so it is drawn
	p_object_->setTexture(&texture_); //Set the texture of the object
	p_object_->setTextureRect(sf::FloatRect(0, 0, p_object_->getScale().x, p_object_->getScale().y)); //Set the texture rect of the object
	return(true);
}

void Obstacle::update(const sf::Vector3f& col)
{
	p_object_->setFillColor(sf::Color(col.x, col.y, col.z)); //Update the colour of the object (Gets it from the map lerping colours)
}

sf::FloatRect Obstacle::getGlobalBounds() const
{
	sf::FloatRect bounds;
	bounds.width = p_object_->getScale().x;
	bounds.height = p_object_->getScale().y;
	bounds.left = (p_object_->getPosition().x + (p_object_->getGlobalBounds().width / 2.f)) - bounds.width / 2.f;
	bounds.top = (p_object_->getPosition().y + (p_object_->getGlobalBounds().height / 2.f)) - bounds.height / 2.f;
	return(bounds);
}
