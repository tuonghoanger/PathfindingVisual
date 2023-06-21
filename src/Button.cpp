#include "Button.h"
#include <iostream>

extern int windowWidth;
extern int windowHeight;

Button::Button() {}

Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string textString,
	sf::Color idle, sf::Color hover) 
{  
	this->buttonState = BTN_IDLE;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));

	this->font = font;
	this->text.setFont(*this->font);
	buttonTittle = textString;
	this->text.setString(textString);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(25);
	this->text.setPosition(
		shape.getPosition().x + (shape.getSize().x - text.getGlobalBounds().width) / 2.f,
		shape.getPosition().y + (shape.getSize().y - text.getGlobalBounds().height) / 2.f
	);

	this->idleColor = idle;
	this->hoverColor = hover;

	this->shape.setFillColor(this->idleColor);
}

Button::~Button() { }

void Button::setOnButtonClickedListener(void(*clickFunc)(std::string))
{
	onButtonClicked = clickFunc;
}

bool Button::isPressed() {
	if (buttonState == BTN_ACTIVE)
	{
		return true;
	}
	return false;
}

void Button::update(const sf::Vector2f mousePos)
{
	buttonState = BTN_IDLE;
	if (shape.getGlobalBounds().contains(mousePos))
	{
		buttonState = BTN_HOVER;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			buttonState = BTN_ACTIVE;
		}
	}

	switch (buttonState)
	{
	case BTN_IDLE:
		shape.setFillColor(idleColor);
		break;
	case BTN_HOVER:
		shape.setFillColor(hoverColor);
		break;
	case BTN_ACTIVE:
		shape.setFillColor(idleColor);
		break;
	default:
		break;
	}
}

void Button::render(sf::RenderTarget* target) {
	target->draw(shape);
	target->draw(text);
}