#include <iostream> 
#include <SFML\Graphics.hpp>
#include "Map\Map.h"
#define REPITITIONS 100000
using namespace std;

int main(void)
{
	Map map;

	sf::Clock c;
	for (int i(0); i < REPITITIONS; ++i)
	{
		map.generateMap();
	}

	cout << "Average Map Generation Time: " << c.restart().asMicroseconds() / REPITITIONS << " Microseconds" << endl;

	system("PAUSE");
	return(0);
}
