#ifndef SQUARE_H
#define SQUARE_H

#include <SFML/Graphics.hpp>

enum square_states { SQUARE_IDLE = 0, SQUARE_HOVER, SQUARE_ACTIVE };

class Square
{
private:
	bool runClickListener = true;



	int posX;
	int posY;
	short unsigned squareState;
	sf::Color idleColor = sf::Color(208, 204, 180);
	sf::Color hoverColor = sf::Color(178, 172, 133);
	
	int edgeSize;
	sf::RectangleShape shape;

	void (*onSquareClicked)(int x,int y);
public:

	Square();
	Square(int edgeLength,int x,int y);
	~Square();

	void setSquareColor(sf::Color color);
	void render(sf::RenderTarget* target);
	void update(const sf::Vector2f mousePos);
	void setOnSquareClickedListener(void (*clickFunc)(int,int));
};

#endif 


