#include "Player.h"



Player::Player()
	: p_object_(nullptr), mass_(3200), invMass_(1.f / mass_), verticalAllowed_(true)
{
	setGravity(sf::Vector2f(0, 981));


}

Player::~Player()
{

}

//Public Functions 
bool Player::initialise()
{
	p_object_->setScale(64.f, 64.f);
	//p_object_->setFillColor(sf::Color::Blue);

	p_object_->setTexture(&texture_);
	p_object_->setTextureRect(sf::FloatRect(0, 0, 128, 128));

	velocity_.x = START_HORIZONTAL_SPEED;

	if (!texture_.loadFromFile("res//spaceship.png"))
		return(false);
	p_object_->setTexture(&texture_);

	return(true);
}

void Player::update(float delta)
{
	stepForces();
	stepVelocity(delta);

	p_object_->move(velocity_ * delta);
	if (velocity_.x < MAX_HORIZONTAL_SPEED)
		velocity_.x += 0.1f;

	force_ = sf::Vector2f(0.f, 0.f);
}

void Player::events(const sf::Event& evnt)
{
	if (evnt.type == sf::Event::KeyPressed)
	{
		if (verticalAllowed_ && evnt.key.code == sf::Keyboard::Space)
		{
			applyForce(sf::Vector2f(0.f, ((-gravity_.y * 200) * mass_))); //* 71.5f));
			verticalAllowed_ = false;
		}
	}

	if (evnt.type == sf::Event::KeyReleased)
	{
		if (!verticalAllowed_ && evnt.key.code == sf::Keyboard::Space)
		{
			verticalAllowed_ = true;
		}
	}
}

void Player::setPosition(sf::Vector2f position)
{
	p_object_->setPosition(position);
}

sf::FloatRect Player::getGlobalBounds() const
{
	sf::FloatRect bounds;
	bounds.width = 64;
	bounds.height = 27;
	bounds.left = (p_object_->getPosition().x + (p_object_->getGlobalBounds().width / 2.f)) - bounds.width / 2.f;
	bounds.top = (p_object_->getPosition().y + (p_object_->getGlobalBounds().height / 2.f)) - bounds.height / 2.f;
	return(bounds);
}

//Private Functions 
void Player::stepForces()
{
	force_ += (gravity_ * mass_);
}

void Player::stepVelocity(float delta)
{

	velocity_ += (invMass_ * force_) * delta; //Increment the velocity by the overall acceleration acting upon the player 
	//printf("\nVel Before: %f", velocity_.y);
	velocity_.y = clamp(velocity_.y, gravity_.y / 2, -gravity_.y / 4);
	//printf("\nVel After: %f", velocity_.y);

}
