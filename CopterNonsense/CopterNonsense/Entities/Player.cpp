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


	velocity_.x = START_HORIZONTAL_SPEED;

	if (!texture_.loadFromFile("res//spaceship.png"))
		return(false);
	if (!reverseTexture_.loadFromFile("res//spaceshipReverse.png"))
		return(false);
	p_object_->setTexture(&texture_);
	//p_object_->setTextureRect(sf::FloatRect(0, 0, 128, 128));
	p_object_->setAlive(true);

	return(true);
}

void Player::update(float delta, const sf::Vector3f& col, const int g)
{
	if (g == 1)
	{
		gravity_.y = 981;
		p_object_->setTexture(&texture_);
	}
	else
	{
		gravity_.y = -981;
		p_object_->setTexture(&reverseTexture_);
	}
	
	stepForces();
	stepVelocity(delta);

	p_object_->move(velocity_ * delta);
	p_object_->setFillColor(sf::Color(col.x, col.y, col.z));
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
		if (evnt.key.code == sf::Keyboard::F)
		{
			gravity_.y *= -1;
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
	/*bounds.left = (p_object_->getPosition().x - bounds.width / 2.f);
	bounds.top = (p_object_->getPosition().y - bounds.height / 2.f);*/
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
	if (gravity_.y > 0)
		velocity_.y = clamp(velocity_.y, gravity_.y / 2, -gravity_.y / 3);
	else
		velocity_.y = clamp(velocity_.y, -gravity_.y / 2, gravity_.y / 5);
	//printf("\nVel After: %f", velocity_.y);

}
