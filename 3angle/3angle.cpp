#include "pch.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include <windows.h>

using namespace std;
using namespace sf;


class point:public CircleShape
{
public:
	int charge;
	point(int positionX, int positionY, int charge);
	~point();

};


class pointsList
{
public:
	vector <point*> points;
	pointsList();
	void addPoint(int positionX, int positionY, int zaryad);
	void drawPoints(RenderWindow &window);
	void drawLines(RenderWindow &window);
	Vector2f * getSumPower(Vector2i position);
};

pointsList::pointsList()
{}

void pointsList::addPoint(int positionX, int positionY, int charge)
{
	point * pointPtr = new point(positionX, positionY, charge);
	this->points.push_back(pointPtr);
}

void pointsList::drawLines(RenderWindow &window)
{
	Vector2i mousePosition = Mouse::getPosition(window);
	Vector2f powerVect = *(this->getSumPower(mousePosition));
	//cout << "powerVect " << powerVect.x << " " << powerVect.y << endl;
	for (int i = 0; i < 100; i++)
	//while ((powerVect.x != 0) && (powerVect.y != 0))
	{
		powerVect = *(this->getSumPower(mousePosition));
		{
			cout << "break" << endl;
			//break;
		}
		sf::Vertex line[] =
		{
			Vertex(Vector2f(mousePosition.x, mousePosition.y)),
			Vertex(Vector2f(mousePosition.x + powerVect.x * 20, mousePosition.y + powerVect.y * 20) )
		};

		mousePosition.x += powerVect.x * 10;
		mousePosition.y += powerVect.y * 10;
		window.draw(line, 2, sf::Lines);
	}

	mousePosition = Mouse::getPosition(window);
	powerVect = *(this->getSumPower(mousePosition));

	for (int i = 0; i < 100; i++)
	//while ((powerVect.x != 0) && (powerVect.y != 0))
	{
		powerVect = *(this->getSumPower(mousePosition));
		if ((powerVect.x == 0) && (powerVect.y == 0))
		{
			cout << "break" << endl;
			//break;
		}

		sf::Vertex line[] =
		{
			Vertex(Vector2f(mousePosition.x, mousePosition.y)),
			Vertex(Vector2f(mousePosition.x - powerVect.x * 20, mousePosition.y - powerVect.y * 20))
		};

		mousePosition.x -= powerVect.x * 10;
		mousePosition.y -= powerVect.y * 10;
		window.draw(line, 2, sf::Lines);
	}

}

Vector2f * pointsList::getSumPower(Vector2i position)
{
	Vector2i powerVect;
	powerVect.x = 0;
	powerVect.y = 0;
	for (int i = 0; i < this->points.size(); i++)
	{
		int X = this->points[i]->getPosition().x;
		int Y = this->points[i]->getPosition().y;
		int deltaX = (position.x - X);
		int deltaY = (position.y - Y);
		
		int distance2 = pow(deltaX, 2) + pow(deltaY, 2);
		int powerAbs = 0;

		if (distance2 > 250)
		{
			powerAbs = (this->points[i]->charge) * 1000000000 / distance2;
		}
		else 
		{
			Vector2f * result = new Vector2f(0, 0);
			return result;
		}
		powerVect = powerVect + Vector2i(powerAbs * deltaX / sqrt(distance2), powerAbs * deltaY / sqrt(distance2));
	}
	//cout << "power X,Y " << powerVect.x << " " << powerVect.y << endl;
	//cout << "power X,Y edin " << powerVect.x / sqrt(pow(powerVect.x, 2) + pow(powerVect.y, 2)) << " " << powerVect.y / sqrt(pow(powerVect.x, 2) + pow(powerVect.y, 2)) << endl;
	Vector2f * result = new Vector2f((float)powerVect.x / sqrt(pow(powerVect.x, 2) + pow(powerVect.y, 2)), (float)powerVect.y / sqrt(pow(powerVect.x, 2) + pow(powerVect.y, 2)));
	//cout << "result float X,Y " << result->x << " " << result->y << endl;
	cout << endl;
	return result;
}

void pointsList::drawPoints(RenderWindow &window)
{
	for (int i = 0; i < this->points.size(); i++)
	{
		window.draw(*this->points[i]);
	}
}

point::point(int positionX, int positionY, int charge)
{
	this->setPosition(positionX, positionY);
	if (charge > 0)
	{
		this->setFillColor(Color::Red);
	}
	else 
	{
		this->setFillColor(Color::Blue);
	}
	this->setRadius(abs(charge * 4));
	this->charge = charge;
	cout << "created point" << endl;
}

point::~point()
{}

void drawVert(RenderWindow &window)
{
	sf::Vertex line[] =
	{
		Vertex(Vector2f(10, 10)),
		Vertex(Vector2f(150, 150))
	};

	window.draw(line, 2, sf::Lines);
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML shapes", sf::Style::Default, settings);//more chetko

	window.setFramerateLimit(10); //speed of discretisation 

	pointsList pointsList;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();			
		}
		window.clear(Color::Black);
		if (Keyboard::isKeyPressed(Keyboard::P))
		{
			Vector2i mousePosition = Mouse::getPosition(window);
			pointsList.addPoint(mousePosition.x, mousePosition.y, 1);
		};
		if (Keyboard::isKeyPressed(Keyboard::N))
		{
			Vector2i mousePosition = Mouse::getPosition(window);
			pointsList.addPoint(mousePosition.x, mousePosition.y, -2);
		};
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			Vector2i mousePosition = Mouse::getPosition(window);
			pointsList.drawLines(window);
		};

		//drawVert(window);

		pointsList.drawPoints(window);
		window.display();
		
		
	}


	cout << "Hello world" << endl;
	return 0;
}