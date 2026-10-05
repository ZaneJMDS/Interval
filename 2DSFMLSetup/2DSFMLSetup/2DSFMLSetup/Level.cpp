#include "Level.h"

Level::Level(std::string _filepath, terrain_types _terrain_type) : filepath(_filepath), terrain_type(_terrain_type)
{
	// Error if can't load image
	if (!world_texture.loadFromFile("Sprites/world_tileset.png"))
	{
		throw "Error loading image";
	}

	if (!platform_texture.loadFromFile("Sprites/platforms.png"))
	{
		throw "Error loading image";
	}

	// Error if can't load image
	if (!enemy_texture.loadFromFile("Sprites/slime_green.png"))
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
			// TILES
			// Wall
			if (level_array[x][y] == 'x')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));

				switch (terrain_type)
				{
				case Forest:

					// Location and size of texture
					if (y != 0 && level_array[x][y - 1] == 'x')
					{
						// Location and size of texture
						NewBox->setTextureRect(sf::IntRect({ 0, 16 }, { 16, 16 })); // Dirt
					}

					else { NewBox->setTextureRect(sf::IntRect({ 0, 0 }, { 16, 16 })); } // Grass

					break;

				case Mountain:

					// Location and size of texture
					if (y != 0 && level_array[x][y - 1] == 'x')
					{
						// Location and size of texture
						NewBox->setTextureRect(sf::IntRect({ 32, 16 }, { 16, 16 })); // Dirt
					}

					else { NewBox->setTextureRect(sf::IntRect({ 32, 0 }, { 16, 16 })); } // Grass

					break;

				case Desert:

					// Location and size of texture
					if (y != 0 && level_array[x][y - 1] == 'x')
					{
						// Location and size of texture
						NewBox->setTextureRect(sf::IntRect({ 64, 16 }, { 16, 16 })); // Dirt
					}

					else { NewBox->setTextureRect(sf::IntRect({ 64, 0 }, { 16, 16 })); } // Grass

					break;

				case Snowy:

					// Location and size of texture
					if (y != 0 && level_array[x][y - 1] == 'x')
					{
						// Location and size of texture
						NewBox->setTextureRect(sf::IntRect({ 96, 16 }, { 16, 16 })); // Dirt
					}

					else { NewBox->setTextureRect(sf::IntRect({ 96, 0 }, { 16, 16 })); } // Grass

					break;
				}
				
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

				switch (terrain_type)
				{
				case Forest:
					// Location and size of texture
					NewBox->setTextureRect(sf::IntRect({ 0, 0 }, { 16, 9 }));

					break;

				case Mountain:
					// Location and size of texture
					NewBox->setTextureRect(sf::IntRect({ 0, 16 }, { 16, 9 }));

					break;

				case Desert:
					// Location and size of texture
					NewBox->setTextureRect(sf::IntRect({ 0, 32 }, { 16, 9 }));

					break;

				case Snowy:
					// Location and size of texture
					NewBox->setTextureRect(sf::IntRect({ 0, 48 }, { 16, 9 }));

					break;
				}
				
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

				// Remember starting position with box class 
				Box boxlogic(NewBox);
				level_boxes.push_back(boxlogic);
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

			// ENEMIES
			if (level_array[x][y] == 'e')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Enemy NewEnemy(start_pos, enemy_texture);

				level_enemies.push_back(NewEnemy);
			}
		}
	}

	// Push all tiles to level tiles
	level_tiles.push_back(level_wall_tiles);
	level_tiles.push_back(level_platform_tiles);
	level_tiles.push_back(level_drum_tiles);
	level_tiles.push_back(level_box_tiles);
	level_tiles.push_back(level_spike_tiles);
	level_tiles.push_back(level_goal_tile);
}

void Level::UnloadLevel(int _final_time)
{
	final_time = _final_time;

	// Clear out all the current tiles in the level
	for (int i = 0; i < level_tiles.size(); i++)
	{
		// Clears specific types of tile
		for (int j = 0; j < level_tiles[i].size(); j++)
		{
			// Delete all tiles that were placed
			delete level_tiles[i][j];
		}
		level_tiles[i].clear();
	}

	level_tiles.clear();

	// Clear boxes as well
	level_boxes.clear();

	// Clear enemies as well
	level_enemies.clear();
}

void Level::Reset(Player* _player)
{
	_player->ResetPosition();

	// Reset all the boxes
	for (int i = 0; i < level_boxes.size(); i++)
	{
		level_boxes[i].ResetPosition(level_box_tiles[i]);
	}

	// Reset all the enemies
	for (int i = 0; i < level_enemies.size(); i++)
	{
		level_enemies[i].Reset();
	}
}
