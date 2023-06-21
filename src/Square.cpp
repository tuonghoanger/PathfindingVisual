#include "Square.h"

Square::Square()
{
}

Square::Square(int edge,int x,int y) : edgeSize(edge),posX(x),posY(y)
{
	squareState = SQUARE_IDLE;
	shape.setSize(sf::Vector2f(edge, edge));
	shape.setPosition(x, y);
	shape.setFillColor(idleColor);
}

Square::~Square() {}

void Square::setSquareColor(sf::Color color)
{
	shape.setFillColor(color);
}

void Square::update(const sf::Vector2f mousePos)
{
	squareState = SQUARE_IDLE;
	if (shape.getGlobalBounds().contains(mousePos))
	{
		squareState = SQUARE_HOVER;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			squareState = SQUARE_ACTIVE;
		}
	}

	switch (squareState)
	{
	case SQUARE_IDLE:
		shape.setFillColor(idleColor);
		break;
	case SQUARE_HOVER:
		shape.setFillColor(hoverColor);
		break;
	case SQUARE_ACTIVE:
		idleColor = sf::Color::Black;
		hoverColor = sf::Color::Black;
		break;
	default:
		break;
	}
}

void Square::setOnSquareClickedListener(void(*clickFunc)(int, int))
{
	onSquareClicked = clickFunc;
}

void Square::render(sf::RenderTarget* target)
{
	target->draw(shape);
}
