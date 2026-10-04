#pragma once
#include "box.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

class Level
{
public:
	// Level dimensions
	static const int level_width = 20;
	static const int level_height = 10;

	// Tiles
	std::vector<sf::RectangleShape*> level_wall_tiles; // Tiles with collision
	std::vector<sf::RectangleShape*> level_platform_tiles; // Tiles to jump and fall through with no side collision
	std::vector<sf::RectangleShape*> level_box_tiles; // Tiles the player can move
	std::vector<sf::RectangleShape*> level_drum_tiles; // Tiles that cause the player to launch with a high velocity
	std::vector<sf::RectangleShape*> level_spike_tiles; // Tiles that kill the player
	std::vector<sf::RectangleShape*> level_goal_tile; // The tile the player needs to reach
	
	// Boxes
	std::vector<Box> level_boxes; // A class to hold the location of each box

	// Textures for said tiles
	sf::Texture world_texture;
	sf::Texture platform_texture;
	sf::Texture drum_texture;
	sf::Texture spike_texture;
	sf::Texture goal_texture;

	std::vector<std::vector<sf::RectangleShape*>> level_tiles; // Remember to add new tiles to LoadLevel

	char level_array[level_width][level_height];

	Level(std::string _filepath);
	~Level();

	void LoadLevel();
	void UnloadLevel();

private:
	std::string filepath;
};

