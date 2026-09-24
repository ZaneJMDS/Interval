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

		// Left of the tile
		if (entityACenter.x <= entityBCenter.x)
		{
			float offset = -(_objA->getGlobalBounds().position.x + _objA->getGlobalBounds().size.x - _objB->getGlobalBounds().position.x);
			_objA->move({ offset, 0 });
		}

		// Right of the tile
		if (entityACenter.x >= entityBCenter.x)
		{
			float offset = (_objB->getGlobalBounds().position.x + _objB->getGlobalBounds().size.x) - _objA->getGlobalBounds().position.x;
			_objA->move(sf::Vector2f(offset, 0));
		}
	}

	// Vertical collision between 2 objects
	static void ResolveYCollisions(sf::Shape* _objA, sf::Shape* _objB, bool _secondObjectWall)
	{
		sf::Vector2f entityACenter = { (_objA->getGlobalBounds().position.x + _objA->getGlobalBounds().size.x / 2.0f),
									  (_objA->getGlobalBounds().position.y + _objA->getGlobalBounds().size.y / 2.0f) };

		sf::Vector2f entityBCenter = { (_objB->getGlobalBounds().position.x + _objB->getGlobalBounds().size.x / 2.0f),
									  (_objB->getGlobalBounds().position.y + _objB->getGlobalBounds().size.y / 2.0f) };

		// Underneath the tile
		if (entityACenter.y <= entityBCenter.y)
		{
			float offset = -(_objA->getGlobalBounds().position.y + _objA->getGlobalBounds().size.y - _objB->getGlobalBounds().position.y);
			_objA->move({ 0, offset });
		}

		// Above the tile
		if (entityACenter.y >= entityBCenter.y)
		{
			float offset = (_objB->getGlobalBounds().position.y + _objB->getGlobalBounds().size.y) - _objA->getGlobalBounds().position.y;
			_objA->move({ 0, offset });
		}
	}
};