#include "Button.h"

// Default button setup
Button::Button(sf::Vector2f _position, sf::Color _color, sf::Text _text)
{
	m_ButtonShape.setPosition(_position);
	m_ButtonShape.setSize({ 150, 50 });
	m_ButtonShape.setFillColor({ _color });
	m_ButtonShape.setOutlineThickness(-2.f);
	m_ButtonShape.setOutlineColor(sf::Color(128, 128, 128)); // Grey
}

Button::~Button()
{
}
