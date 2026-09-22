#include "Level.h"

Level::Level(int _level_width, int _level_height)
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

	LoadLevel("Levels/Level1.txt");
}

Level::~Level()
{
}

void Level::LoadLevel(std::string _file_path)
{
	// Open the file from this path
	std::fstream load_file_stream;
	load_file_stream.open(_file_path, std::ios::in);

	std::string load_file_string;
	int line_count = 0;

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
				NewBox->setFillColor(sf::Color::White);

				// Collider logic
				level_wall_tiles.push_back(NewBox);
			}

			// Platform
			if (level_array[x][y] == 'p')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				NewBox->setFillColor(sf::Color::Blue);

				// Collider logic
				level_platform_tiles.push_back(NewBox);
			}
		}
	}
}

void Level::UnloadLevel()
{
}
