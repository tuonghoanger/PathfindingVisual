#include <SFML/Graphics.hpp>
#include <iostream> 
#include "Button.h"
#include "Square.h"
#include "Grid.h"
#include <set>

sf::RenderWindow window;
int windowWidth, windowHeight;
Button buttonStart;
Button buttonRandom;
sf::Font font;
sf::Text exitText;
sf::Vector2f mousePos;
Grid grid;
bool enableRdBt = true;
bool enableStBt = true;

void init() {
	windowWidth = window.getSize().x;
	windowHeight = window.getSize().y;
	font.loadFromFile("usingFont.ttf");
	buttonStart = Button(windowWidth * 1 / 10, windowHeight * 12 / 14, windowWidth / 10 , windowHeight / 10,
		&font, "START", sf::Color(28, 28, 28), sf::Color(54, 54, 54));

	buttonRandom = Button(windowWidth * 3 / 10, windowHeight * 12 / 14, windowWidth / 10, windowHeight / 10,
		&font, "RANDOM", sf::Color(28, 28, 28), sf::Color(54, 54, 54));

	int start = windowWidth / 30;

	exitText.setFont(font);
	exitText.setCharacterSize(35);
	exitText.setString("Exit");
	exitText.setPosition(windowWidth * 9 / 10, windowHeight * 9 / 10);
	exitText.setFillColor(sf::Color::Black);
}

void draw() {
	window.clear(sf::Color(224, 220, 220));
	
	window.draw(exitText);

	buttonStart.render(&window);
	buttonRandom.render(&window);
	grid.renderGrid(&window); 
	window.display();
}

void input() {
	sf::Event ev;
	while (window.pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		default:
			break;
		}

		if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) //Mouse button Pressed
		{
			if (exitText.getGlobalBounds().contains(mousePos)) window.close();
			if (buttonRandom.isPressed() && enableRdBt)
			{
				grid.randomGrid();
			}
			if (buttonStart.isPressed() && enableStBt)
			{
				grid.doPathFind();
			}
		}
	}
}

void update() {
	mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
	buttonStart.update(mousePos);
	buttonRandom.update(mousePos);
}

void doSfml() {
	window.create(sf::VideoMode::getDesktopMode(), "Pathfinding", sf::Style::None);
	//window.create(sf::VideoMode(1600, 900), "Pathfinding");
	window.setFramerateLimit(60);
	init();

	while (window.isOpen())
	{
		draw();
		update();
		input();
	}
}

int main() {
	doSfml();

	return 0;
}