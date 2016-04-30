#include "Bullet.h"
Bullet::Bullet()
	: mass_(3400), invMass_(1.f / mass_)
{
	setGravity(sf::Vector2f(0, 450));
}

bool Bullet::initialise()
{
	elapsedTime_ = 0;
	p_object_->setScale(15.f, 8.f);
	p_object_->setAlive(false);
	p_object_->setFillColor(sf::Color::Red);
	return true;
}

void Bullet::setup(const sf::Vector2f& rotation, const sf::Vector2f& position)
{
	float rot = (Degrees(atan2(rotation.y, rotation.x)));
	p_object_->setPosition(position);
	p_object_->setRotation(rot);
	p_object_->setAlive(true);
	velocity_ = sf::Vector2f(0, 0);
	applyForce(sf::Vector2f(rotation.x * 150, rotation.y * 150)*mass_);
}

void Bullet::update(float dtime)
{
	if (p_object_->getAlive())
	{
		stepForces();
		stepVelocity(dtime);

		p_object_->move(velocity_ * dtime);

		force_ = sf::Vector2f(0.f, 0.f);
	}
}

void Bullet::stepForces()
{
	force_ += (gravity_ * mass_);
}

void Bullet::stepVelocity(float delta)
{
	velocity_ += (invMass_ * force_) * delta; 
}
