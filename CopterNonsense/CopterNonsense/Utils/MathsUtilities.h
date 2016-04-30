#ifndef MATH_UTILITIES_H
#define MATH_UTILITIES_H
#include <SFML\Graphics.hpp> 
#define PI 3.14159265359
/*
Header file of mathematical utilitiy functions to aid in calculations.
-Written by: M. Walker
*/


inline float Degrees(float theta)
{//Will convert radians into degrees
	return(static_cast<float> (theta * 180.0 / PI));
}

inline float Radians(float theta)
{//Will convert degrees into radians
	return (static_cast<float> (theta * PI / 180.0));
}

inline sf::Vector2f Normalize(sf::Vector2f v)
{//Will normalize a vector
	float length(sqrtf((v.x * v.x) + (v.y * v.y)));

	if (length != 0.f)
	{
		v.x /= length;
		v.y /= length;
	}
	return(v);
}

inline float GetLength(sf::Vector2f v)
{//Get the magnitude/length of a vector
	return(sqrtf((v.x * v.x) + (v.y * v.y)));
}

inline float GetSquareLength(sf::Vector2f v)
{
	return((v.x * v.x) + (v.y * v.y));
}

inline float DotProduct(sf::Vector2f a, sf::Vector2f b)
{//Return dot product of two vectors
	return((a.x * b.x) + (a.y * b.y));
}

inline sf::Vector2f operator*(const sf::Vector2f& v, float scale)
{//Overload operator for Vector2f
	return(sf::Vector2f(v.x * scale, v.y * scale));
}

inline sf::Vector2f operator+(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return(sf::Vector2f((a.x + b.x), (a.y + b.y)));
}

inline sf::Vector2f operator/(const sf::Vector2f& a, float scale)
{
	return(sf::Vector2f(a.x / scale, a.y / scale));
}
inline sf::Vector2f operator+=(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return(sf::Vector2f(a.x + b.x, a.y + b.y));
}

inline sf::Vector2f RotateVector(sf::Vector2f v, float angle)
{//Rotate a vector by an angle (given in degrees)
	float x = v.x * cosf(Radians(angle)) - v.y * sinf(Radians(angle));
	float y = v.x * sinf(Radians(angle)) + v.y * cosf(Radians(angle));
	return(sf::Vector2f(x, y));
}

inline sf::Vector2f subtract(sf::Vector2f a, sf::Vector2f b)
{
	return(sf::Vector2f(a.x - b.x, a.y - b.y));
}


inline float clamp(float value, float max, float min)
{
	if (value > max)
		return max;
	
	if (value < min)
		return min;

	return(value);
}

#endif
