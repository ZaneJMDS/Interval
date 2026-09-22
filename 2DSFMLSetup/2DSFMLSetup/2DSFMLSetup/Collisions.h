#pragma once
#include "SFML/Graphics.hpp"

static class Collisions
{
public:
	static void ResolveXCollisions(sf::Shape* _objA, sf::Shape* _objB, bool _secondObjectWall)
	{
		sf::Vector2f entityACenter = { (_objA->getGlobalBounds().position.x + _objA->getGlobalBounds().size.x / 2.0f),
									  (_objA->getGlobalBounds().position.y + _objA->getGlobalBounds().size.y / 2.0f) };

		sf::Vector2f entityBCenter = { (_objB->getGlobalBounds().position.x + _objB->getGlobalBounds().size.x / 2.0f),
									  (_objB->getGlobalBounds().position.y + _objB->getGlobalBounds().size.y / 2.0f) };

		if (entityACenter.x <= entityBCenter.x) // to the left
		{
			float offset = -(_objA->getGlobalBounds().position.x + _objA->getGlobalBounds().size.x - _objB->getGlobalBounds().position.x);
			_objA->move({ offset, 0 });
		}

		if (entityACenter.x >= entityBCenter.x) // to the right
		{
			float offset = (_objB->getGlobalBounds().position.x + _objB->getGlobalBounds().size.x) - _objA->getGlobalBounds().position.x;
			_objA->move(sf::Vector2f(offset, 0));
		}
	}
	static void ResolveYCollisions(sf::Shape* _objA, sf::Shape* _objB, bool _secondObjectWall)
	{
		sf::Vector2f entityACenter = { (_objA->getGlobalBounds().position.x + _objA->getGlobalBounds().size.x / 2.0f),
									  (_objA->getGlobalBounds().position.y + _objA->getGlobalBounds().size.y / 2.0f) };

		sf::Vector2f entityBCenter = { (_objB->getGlobalBounds().position.x + _objB->getGlobalBounds().size.x / 2.0f),
									  (_objB->getGlobalBounds().position.y + _objB->getGlobalBounds().size.y / 2.0f) };

		// underneath the tile
		if (entityACenter.y <= entityBCenter.y)
		{
			float offset = -(_objA->getGlobalBounds().position.y + _objA->getGlobalBounds().size.y - _objB->getGlobalBounds().position.y);
			_objA->move({ 0, offset });
		}

		// above the tile. 
		if (entityACenter.y >= entityBCenter.y)
		{
			float offset = (_objB->getGlobalBounds().position.y + _objB->getGlobalBounds().size.y) - _objA->getGlobalBounds().position.y;
			_objA->move({ 0, offset });
		}
	}
};