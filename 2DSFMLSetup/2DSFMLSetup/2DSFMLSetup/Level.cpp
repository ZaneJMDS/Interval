#include "Level.h"

Level::Level(std::string _filepath) : filepath(_filepath)
{
	for (int i = 0; i < level_width; i++)
	{
		for (int j = 0; j < level_height; j++)
		{
			// TODO Draw a wall
			if (i == 0 || j == 0)
			{

			}
		}
	}


	// Error if can't load image
	if (!world_texture.loadFromFile("Sprites/world_tileset.png"))
	{
		throw "Error loading image";
	}

	if (!platform_texture.loadFromFile("Sprites/platforms.png"))
	{
		throw "Error loading image";
	}
}

Level::~Level()
{
}

void Level::LoadLevel()
{
	// Open the file from this path
	std::fstream load_file_stream;
	load_file_stream.open(filepath, std::ios::in);

	std::string load_file_string;
	int line_count = 0;

	// Load file to 2D Array
	if (load_file_stream.is_open())
	{
		// collumns
		while (std::getline(load_file_stream, load_file_string))
		{
			// rows
			for (int i = 0; i < load_file_string.size(); i++)
			{
				level_array[i][line_count] = load_file_string[i];
			}
			line_count++;
		}

		load_file_stream.close();
	}

	// collumns
	for (int y = 0; y < level_height; y++)
	{
		// rows
		for (int x = 0; x < level_width; x++)
		{
			// Wall
			if (level_array[x][y] == 'x')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));

				// Location and size of texture
				NewBox->setTextureRect(sf::IntRect({ 0, 0 }, { 16, 16 }));
				NewBox->setTexture(&world_texture);

				// Collider logic
				level_wall_tiles.push_back(NewBox);
			}

			// Platform
			if (level_array[x][y] == 'p')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 32 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));

				// Location and size of texture
				NewBox->setTextureRect(sf::IntRect({ 0, 0 }, { 16, 9 }));
				NewBox->setTexture(&platform_texture);

				// Collider logic
				level_platform_tiles.push_back(NewBox);
			}

			// Box
			if (level_array[x][y] == 'b')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				
				// Location and size of texture
				NewBox->setTextureRect(sf::IntRect({ 112, 48 }, { 16, 16 }));
				NewBox->setTexture(&world_texture);

				// Collider logic
				level_box_tiles.push_back(NewBox);
			}

			// Drum
			if (level_array[x][y] == 'd')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				
				NewBox->setFillColor(sf::Color::Green);// Spawn a box at current location
				
				// Collider logic
				level_drum_tiles.push_back(NewBox);
			}

			// Spike
			if (level_array[x][y] == 's')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				
				NewBox->setFillColor(sf::Color::Red);// Spawn a box at current location

				level_spike_tiles.push_back(NewBox);
			}

			// Goal
			if (level_array[x][y] == 'g')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				
				NewBox->setFillColor(sf::Color::Yellow);// Spawn a box at current location

				level_goal_tile.push_back(NewBox);
			}
		}
	}

	// Push all tiles to level tiles
	level_tiles.push_back(level_wall_tiles);
	level_tiles.push_back(level_platform_tiles);
	level_tiles.push_back(level_box_tiles);
	level_tiles.push_back(level_drum_tiles);
	level_tiles.push_back(level_spike_tiles);
	level_tiles.push_back(level_goal_tile);
}

void Level::UnloadLevel()
{
	// Clear out all the current tiles in the level
	for (int i = 0; i < level_tiles.size(); i++)
	{
		// Clears specific types of tile
		for (int j = 0; j < level_tiles[i].size(); j++)
		{
			// Delete all boxes that were placed
			delete level_tiles[i][j];
		}
		level_tiles[i].clear();
	}

	level_tiles.clear();
}
