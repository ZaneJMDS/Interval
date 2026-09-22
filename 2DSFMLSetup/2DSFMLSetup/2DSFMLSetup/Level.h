#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

class Level
{
public:
	// Level dimensions
	static const int level_width = 15;
	static const int level_height = 10;

	// Tiles
	std::vector<sf::RectangleShape*> level_tiles; // Tiles with no collision
	std::vector<sf::RectangleShape*> level_wall_tiles; // Tiles with collision

	char level_array[level_width][level_height];

	Level(int _level_width, int _level_height);
	~Level();

	void LoadLevel(std::string _file_path);
	void UnloadLevel();
};

