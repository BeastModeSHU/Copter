#include "Player.h"

Player::Player() //Initialise important variables
	: p_object_(nullptr), mass_(3200), invMass_(1.f / mass_), verticalAllowed_(true)
{
	setGravity(sf::Vector2f(0, 981)); //Set the initial value of gravity
}

//Public Functions 
bool Player::initialise()
{
	if (!texture_.loadFromFile("res//spaceship.png")) //Try and load in the textures, if it fails return false and halt the program
		return(false);
	if (!reverseTexture_.loadFromFile("res//spaceshipReverse.png"))
		return(false);

	p_object_->setScale(64.f, 64.f); //Set the scale of the player

	velocity_.x = START_HORIZONTAL_SPEED; //Set the initial speed of the player

	p_object_->setTexture(&texture_); //Set the texture of the player
	p_object_->setAlive(true); //Set the player to alive so it is drawn

	return(true);
}

void Player::update(float delta, const sf::Vector3f& col, const int g, const bool started)
{
	//If gravity is normal
	if (g == 1) 
	{ 
		gravity_.y = 981; //Set gravity to act downwards
		p_object_->setTexture(&texture_); //Set the texture to normal
	}
	else
	{
		gravity_.y = -981; //Set gravity to act upwards
		p_object_->setTexture(&reverseTexture_); //Set the texture to the reverse to make it look upside down
	}

	p_object_->setFillColor(sf::Color(col.x, col.y, col.z)); //Set the colour of the player to the same as the lerping map
	
	//If the game has started (Player has pressed spacebar to start)
	if (started)
	{
		stepForces(); //Update the forces
		stepVelocity(delta); //Update the velocity of the palyer

		p_object_->move(velocity_ * delta); //Move the player object

		//If the velocity is less than the max speed
		if (velocity_.x < MAX_HORIZONTAL_SPEED)
			velocity_.x += 0.1f; //Increment the current x velocity

		force_ = sf::Vector2f(0.f, 0.f); //Reset the force to (0,0)
	}
}

void Player::events(const sf::Event& evnt)
{
	//If there is a key press event
	if (evnt.type == sf::Event::KeyPressed)
	{
		//If the key is the spacebar and you are allowed to jump
		if (verticalAllowed_ && evnt.key.code == sf::Keyboard::Space)
		{
			applyForce(sf::Vector2f(0.f, ((-gravity_.y * 200) * mass_))); //* 71.5f)); //Apply a force in the opposite direction of gravity
			verticalAllowed_ = false; //Disalllow another jump
		}
	}

	//If there is a key released event
	if (evnt.type == sf::Event::KeyReleased)
	{
		//If the key is the spacebar and you arent allowed to jump
		if (!verticalAllowed_ && evnt.key.code == sf::Keyboard::Space)
		{
			verticalAllowed_ = true; //Allow you to jump again
		}
	}
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
	force_ += (gravity_ * mass_); //Update the current force by gravity
}

void Player::stepVelocity(float delta)
{
	velocity_ += (invMass_ * force_) * delta; //Increment the velocity by the overall acceleration acting upon the player 
	//If the gravity is acting downwards
	if (gravity_.y > 0)
		velocity_.y = clamp(velocity_.y, gravity_.y / 2, -gravity_.y / 3); //Clamp y-velocity of the player (So it doesnt move to fast)
	else
		velocity_.y = clamp(velocity_.y, -gravity_.y / 2, gravity_.y / 5); //Do the same but when gravity is acting the opposite way

}
