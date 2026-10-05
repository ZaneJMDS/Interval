#pragma once
#include "box.h"
#include "enemy.h"
#include "player.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

enum terrain_types
{
	Forest,
	Desert,
	Mountain,
	Snowy
};

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

	// Enemies
	std::vector<Enemy> level_enemies; // Default enemy that walks back and forward
	std::vector<sf::RectangleShape*> level_copycat; // Enemy that copies the player's movement
	
	// Boxes
	std::vector<Box> level_boxes; // A class to hold the location of each box

	// Textures for said tiles
	sf::Texture world_texture;
	sf::Texture platform_texture;
	sf::Texture drum_texture;
	sf::Texture spike_texture;
	sf::Texture goal_texture;

	// text
	sf::Texture enemy_texture;

	std::vector<std::vector<sf::RectangleShape*>> level_tiles; // Remember to add new tiles to LoadLevel

	char level_array[level_width][level_height];

	Level(std::string _filepath, terrain_types _terrain_type);
	~Level();

	void LoadLevel();
	void UnloadLevel(int _final_time);
	void Reset(Player* _player);
	int GetTime() { return final_time; }

private:
	int final_time;
	std::string filepath;
	terrain_types terrain_type;
};

