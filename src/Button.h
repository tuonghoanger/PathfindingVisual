#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

enum button_states{BTN_IDLE = 0,BTN_HOVER,BTN_ACTIVE}; 

class Button
{
private:
	short unsigned buttonState;
	bool runClickListener = true;

	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text text;

	sf::Color idleColor;
	sf::Color hoverColor;

	std::string buttonTittle;
	void (*onButtonClicked)(std::string buttonTitle);

public:
	Button();
	Button(float x, float y, float width, float height,
		sf::Font* font, std::string text,
		sf::Color idle,sf::Color hover);
	~Button();

	void setOnButtonClickedListener(void (*clickFunc)(std::string));
	void update(const sf::Vector2f mousePos);
	void render(sf::RenderTarget* target);
	bool isPressed();
};

#endif // !BUTTON_H