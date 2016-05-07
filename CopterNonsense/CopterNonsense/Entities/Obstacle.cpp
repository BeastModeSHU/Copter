#include "Obstacle.h"

Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
}

bool Obstacle::initialise()
{

	if (!texture_.loadFromFile("res//obstacle.png"))
		return(false);
	texture_.setRepeated(true);

	p_object_->setScale(sf::Vector2f(25, (rand() % 100 + 75)));
	p_object_->setPosition(0, 0);
	p_object_->setAlive(true);
	p_object_->setTexture(&texture_);
	p_object_->setTextureRect(sf::FloatRect(0, 0, p_object_->getScale().x, p_object_->getScale().y));
	//p_object_->setFillColor(sf::Color::Red);

	return true;
}

void Obstacle::update(const sf::Vector3f& col)
{
	p_object_->setFillColor(sf::Color(col.x, col.y, col.z));
}

sf::FloatRect Obstacle::getGlobalBounds() const
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
