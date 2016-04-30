#include "GameObject.h"

GameObject::GameObject()
	: shape_(GameObjectPrimitive::Box), texture_(nullptr)
{
	vertexArray_.clear();
	vertexArray_.setPrimitiveType(sf::Quads);
	createSquare();
}

GameObject::GameObject(GameObjectPrimitive shape)
	: shape_(shape), texture_(nullptr)
{
	vertexArray_.clear();
	//Check the shape type
	switch (shape_)
	{
	case GameObjectPrimitive::Box:
		vertexArray_.setPrimitiveType(sf::Quads);//setup vertex array primitives to be a quad
		createSquare();
		break;
	case GameObjectPrimitive::Circle:
		createSquare(); //for AABB dimensions
		createCircle();

	}

}


GameObject::~GameObject()
{

}

void GameObject::draw(sf::RenderTarget& rTarget, sf::RenderStates rStates) const
{//Will draw the game object to it's render target

	rStates.transform *= getTransform(); //apply the transformations applied to an instance of the game ojbect to the render states

	if (texture_)
		rStates.texture = texture_;

	if (shape_ != GameObjectPrimitive::Circle)
		rTarget.draw(vertexArray_, rStates); //Draw a sf::VertexArray with the transformations applied
	else
	{
		rTarget.draw(circleShape_, rStates);
	}
}

//-- Functions to setup primitives -- 
void GameObject::createSquare()
{
	sf::Vertex vertices[4];

	vertices[0].position = sf::Vector2f(0.f, 0.f);
	vertices[1].position = sf::Vector2f(1.f, 0.f);
	vertices[2].position = sf::Vector2f(1.f, 1.f);
	vertices[3].position = sf::Vector2f(0.f, 1.f);

	for (int i(0); i < 4; ++i)
	{//Loop through, change the colour of each vertex and append it to the array

		vertexArray_.append(vertices[i]);
	}
}

void GameObject::createCircle(unsigned int pointCount)
{
	circleShape_ = sf::CircleShape(1.f, pointCount);
	circleShape_.setOrigin(0.5f, 0.5f);
	circleShape_.setPosition(0.f, 0.f);
	circleShape_.setFillColor(sf::Color::Blue);
}
//--End of primitive setup

//Texture related
void GameObject::setTexture(const sf::Texture* texture)
{//Set the texture pointer to a new pointer
	assert(texture);//sanity check: texture not null
	texture_ = texture;
	sf::FloatRect rect;
	rect.left = 0;
	rect.top = 0;
	rect.width = static_cast<float> (texture->getSize().x);
	rect.height = static_cast<float> (texture->getSize().y);
	setTextureRect(rect);
}

void GameObject::setTextureRect(const sf::FloatRect& rect)
{
	switch (shape_)
	{
	case GameObjectPrimitive::Box:
	{//If we've got a primitive of a square
		vertexArray_[0].texCoords = sf::Vector2f(rect.left, rect.top);
		vertexArray_[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
		vertexArray_[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
		vertexArray_[3].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
	}
	case GameObjectPrimitive::Circle:
		circleShape_.setTextureRect(sf::IntRect{ static_cast<int> (rect.left), static_cast<int> (rect.top), static_cast<int> (rect.width), static_cast<int> (rect.height) });
		break;
	}
}

sf::FloatRect GameObject::getLocalBounds() const
{
	sf::FloatRect local;
	switch (shape_)
	{
	case Box:
		local.left = vertexArray_[0].position.x;
		local.top = vertexArray_[0].position.y;
		local.width = 1.f;
		local.height = 1.f;
		break;
	case Circle:
		local.width = 1.8f;
		local.height = 1.8f;
		local.left = vertexArray_[0].position.x - local.width / 2.f;
		local.top = vertexArray_[0].position.y - local.height / 2.f;

		break;
	}
	return(local);
}

sf::FloatRect GameObject::getGlobalBounds() const
{
	return(getTransform().transformRect(getLocalBounds())); //return the transformed local bounds of the shape
}

void GameObject::setFillColor(sf::Color col)
{//Set the fill colour of a game object
	if (shape_ != GameObjectPrimitive::Circle)
	{
		for (int i(0); i < vertexArray_.getVertexCount(); ++i)
			vertexArray_[i].color = col;
	}
	else
	{

		circleShape_.setFillColor(col);
	}
}

void GameObject::setAlive(bool b)
{
	alive_ = b;
}

void GameObject::setAnimation(const Animation& a)
{
	animation_ = &a;
	texture_ = animation_->getSpriteSheet();
	stopAnimation();
}

void GameObject::isAnimated(bool b)
{
	isAnimated_ = b;
}

void GameObject::updateAnimation(const sf::Time& delta)
{
	if (!paused_ && animation_)
	{//if not paused and the animation isn't null

		animationTimer_ += delta;

		if (animationTimer_ > frameTime_)
		{//if the time in the current frame > frame time
			animationTimer_ = sf::microseconds(animationTimer_.asMicroseconds() % frameTime_.asMicroseconds());

			if (currentFrame_ + 1 < static_cast<int>(animation_->getSize()))
				++currentFrame_;
			else
			{
				if (!loopAnimation_)
				{
					paused_ = true;
				}
				else
				{

					currentFrame_ = 0;
				}

			}
		}
		setFrame(currentFrame_, false);
	}
}

void GameObject::setFrameTime(const sf::Time& frameTime)
{
	frameTime_ = frameTime;
}

void GameObject::setFrame(int index, bool reset)
{
	if (animation_)
	{

		sf::IntRect rect = animation_->getFrame(index);
		float left, top, width, height;
		left = static_cast<float>(rect.left);
		top = static_cast<float>(rect.top);
		width = static_cast<float>(rect.width);
		height = static_cast<float>(rect.height);

		vertexArray_[0].position = sf::Vector2f(0.f, 0.f);
		vertexArray_[1].position = sf::Vector2f(1.f, 0.f);
		vertexArray_[2].position = sf::Vector2f(1.f, 1.f);
		vertexArray_[3].position = sf::Vector2f(0.f, 1.f);

		vertexArray_[0].texCoords = sf::Vector2f(left, top);
		vertexArray_[1].texCoords = sf::Vector2f(left + width, top);
		vertexArray_[2].texCoords = sf::Vector2f(left + width, top + height);
		vertexArray_[3].texCoords = sf::Vector2f(left, top + height);

	}
	if (reset)
		animationTimer_ = sf::Time::Zero;
}

void GameObject::playAnimation()
{
	paused_ = false;
}

void GameObject::pauseAnimation()
{
	paused_ = true;
}

void GameObject::stopAnimation()
{
	currentFrame_ = 0;
	animationTimer_ = sf::Time(sf::seconds(0.f));
}

void GameObject::setAnimationLoop(bool a)
{
	loopAnimation_ = a;
}
