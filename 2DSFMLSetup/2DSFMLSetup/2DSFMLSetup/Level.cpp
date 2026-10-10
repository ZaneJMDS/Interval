#include "Level.h"

Level::Level(std::string _filepath, terrain_types _terrain_type) : filepath(_filepath), terrain_type(_terrain_type)
{
	// load Textures and fonts here
	if (!world_texture.loadFromFile("Sprites/world_tileset.png"))
	{
		throw "Error loading image";
	}

	if (!platform_texture.loadFromFile("Sprites/platforms.png"))
	{
		throw "Error loading image";
	}

	if (!enemy_texture.loadFromFile("Sprites/slime_green.png"))
	{
		throw "Error loading image";
	}

	if (!goal_texture.loadFromFile("Sprites/fruit.png"))
	{
		throw "Error loading image";
	}

	if (!drum_texture.loadFromFile("Sprites/spring.png"))
	{
		throw "Error loading image";
	}

	if (!spike_texture.loadFromFile("Sprites/spike.png"))
	{
		throw "Error loading image";
	}

	if (!Font1.openFromFile("PressStart2P-Regular.ttf"))
	{
		throw "Font could not be loaded";
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
				// Create a box at current location
				sf::Vector2f start_pos(x * 64, y * 64);
				Box NewBox(start_pos);
				NewBox.box_shape.setTexture(&world_texture);
				level_boxes.push_back(NewBox);
			}

			// Drum
			if (level_array[x][y] == 'd')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				
				NewBox->setTexture(&drum_texture);

				// Collider logic
				level_drum_tiles.push_back(NewBox);
			}

			// Spike
			if (level_array[x][y] == 's')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));

				NewBox->setTexture(&spike_texture);

				level_spike_tiles.push_back(NewBox);
			}

			// Goal
			if (level_array[x][y] == 'g')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				
				switch (terrain_type)
				{
				case Forest:
				{
					NewBox->setTextureRect(sf::IntRect({ 0, 0 }, { 16, 16 }));
				}

				break;

				case Mountain:
				{
					NewBox->setTextureRect(sf::IntRect({ 0, 16 }, { 16, 16 }));
				}

				break;

				case Desert:
				{
					NewBox->setTextureRect(sf::IntRect({ 0, 32 }, { 16, 16 }));
				}

				break;

				case Snowy:
				{
					NewBox->setTextureRect(sf::IntRect({ 0, 48 }, { 16, 16 }));
				}
				break;
				}

				NewBox->setTexture(&goal_texture);

				level_goal_tile.push_back(NewBox);
			}

			// ENEMIES
			if (level_array[x][y] == 'e')
			{
				sf::Vector2f start_pos(x * 64, y * 64 + 40);
				Enemy NewEnemy(start_pos);

				NewEnemy.enemy_shape.setTexture(&enemy_texture);

				level_enemies.push_back(NewEnemy);
			}
		}
	}

	// Push all tiles to level tiles
	level_tiles.push_back(level_wall_tiles);
	level_tiles.push_back(level_platform_tiles);
	level_tiles.push_back(level_drum_tiles);
	level_tiles.push_back(level_spike_tiles);
	level_tiles.push_back(level_goal_tile);
}

void Level::UnloadLevel()
{
	// Get the player's final time
	if (current_time < final_time)
	{
		final_time = current_time;
	}

	// Clear out all the current tiles in the level
	for (int i = 0; i < level_tiles.size(); i++)
	{
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
		level_boxes[i].ResetPosition();
	}

	// Reset all the enemies
	for (int i = 0; i < level_enemies.size(); i++)
	{
		level_enemies[i].Reset();
	}

	// Restart player's time for this room
	stopwatch.restart();
}

sf::Text Level::StopwatchUpdate()
{
	// Update stopwatch and display to screen
	sf::Time elapsed2 = stopwatch.getElapsedTime();
	current_time = (elapsed2.asSeconds());
	sf::Text ClockText(Font1, "TIME: " + std::to_string(current_time), 32);
	ClockText.setPosition({ 150.f, 650.f });

	return ClockText;
}
