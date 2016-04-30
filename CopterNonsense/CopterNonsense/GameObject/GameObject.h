#pragma once
#include <SFML\Graphics.hpp>
#include <assert.h>
#include <iostream>

//Game includes

#include "..\Utils\MathsUtilities.h"
#include "..\Utils\Animation.hpp"

/*
Default game object. Blueprints the behaviour of all game objects
-Written by: M. Walker
*/

enum GameObjectPrimitive
{//Game object primitive renderable shapes that will be textured
	Box,
	Circle
};

class GameObject
	: public sf::Drawable, public sf::Transformable
{
public:
	GameObject();
	GameObject(GameObjectPrimitive shape);

	virtual ~GameObject();
	bool init();
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const; //overridden from drawable
	void setTexture(const sf::Texture*);
	void setTextureRect(const sf::FloatRect& rect); //set the area of the texture we want to be displayed on the shape
	void setFillColor(sf::Color);
	void setAlive(bool);
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	bool getAlive() const { return alive_; }

	//Get what type of shape this game object is (determines it's collider)
	GameObjectPrimitive getObjectPrimitive() const{ return (shape_); }

	const sf::Texture* getTexture() const { return texture_; }

	void setCirclePointCount(unsigned int pointCount)
	{ //if the object is a circle, set the point count
		if (shape_ == Circle)
			createCircle(pointCount);
	}
	float getRadius() const{

		return(getScale().x);
	};
	void setVelocity(sf::Vector2f v) { velocity_ = v; }
	sf::Vector2f getVelocity() const { return velocity_; }

	void setAnimation(const Animation&);
	void isAnimated(bool);
	void updateAnimation(const sf::Time&);
	void setFrameTime(const sf::Time&);
	void setFrame(int, bool = true);
	void playAnimation();
	void pauseAnimation();
	void stopAnimation();
	void setAnimationLoop(bool);


private:
	//Functions
	void createSquare(); //setup the class to be a square as a primitive
	void createCircle(unsigned int pointCount = 30);
private:
	const GameObjectPrimitive shape_; //The default shape which will be textured
	sf::VertexArray vertexArray_; //Vertex array to compose the default shape
	const sf::Texture* texture_; //The texture that will be applied to the 
	sf::CircleShape circleShape_;	 //For ease, use the SFML circle shape
	sf::Vector2f velocity_;
	bool alive_;

	//Animation stuff
	const Animation* animation_;
	sf::Time animationTimer_;
	sf::Time frameTime_;
	int currentFrame_;
	bool isAnimated_;
	bool paused_;
	bool loopAnimation_;
};
