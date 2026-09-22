#include <SFML/Graphics.hpp>
#include "Collisions.h"
#include "Level.h"
#include "Physics.h"

// Update player's position
float UpdatePlayer(float _playerYvel, float _Yvelocity, float _dt)
{
    if (_playerYvel < 4.f) { _playerYvel += _Yvelocity * _dt; }
    return _playerYvel;
}

int main()
{
    // Set window settings
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Interval");
    window.setFramerateLimit(60);

    // Setup player sprite
    float player_size = 50.f;
    sf::RectangleShape player_shape(sf::Vector2f(player_size, player_size));
    player_shape.setPosition({ 200.f, 450.f });
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("Meatboy.png")) {
        // perror("Couldn't load texture \"myTexture\".");
        return 0;
    }
    player_shape.setTexture(&player_texture);
    player_shape.setOrigin(sf::Vector2f(player_size / 2.f, player_size / 2.f));

    bool gravity = true;

    float playerY_vel = 0.f;
    float playerX_vel = 0.f;

    // Load the level
    Level MainLevel(15, 10);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // If close button pressed closed the window
            if (event->is<sf::Event::Closed>()) { window.close(); }

            // Swap gravity direction
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::G)
                {
                    gravity = !gravity;
                    player_shape.rotate(sf::degrees(180));
                }
            }
        }

        playerX_vel = 0.f; // Reset player's X velocity if they stop moving left and right

        // KEYBINDS
        // Reset player
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {
            player_shape.setPosition({ 200.f, 450.f });
        }

        // HORIZONTAL MOVEMENT
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            playerX_vel = -5;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            playerX_vel = 5;
        }

        // Normal gravity
        if (gravity)
        {
            playerY_vel = UpdatePlayer(playerY_vel, 1.f, 0.0167f);
            player_shape.move({ 0, playerY_vel });
        }

        // Reversed gravtity
        else
        {
            playerY_vel = UpdatePlayer(playerY_vel, -1.f, 0.0167f);
            player_shape.move({ 0, playerY_vel });
        }

        // COLLISION PROCESSING - Y
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            if (player_shape.getGlobalBounds().findIntersection(MainLevel.level_wall_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveYCollisions(&player_shape, MainLevel.level_wall_tiles[i], false);
                playerY_vel = 0.f; // Set the players Y velocity to 0 if they are colliding with an object

                // VERTICAL MOVEMENT
                // Jump
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                {
                    playerY_vel = -2.5;
                }
            }
        }

        // Move on X
        player_shape.move({ playerX_vel, 0 });

        // COLLISION PROCESSING - X
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            if (player_shape.getGlobalBounds().findIntersection(MainLevel.level_wall_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveXCollisions(&player_shape, MainLevel.level_wall_tiles[i], false);
            }
        }

        window.clear();

        // Draw all the level tiles
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            window.draw(*MainLevel.level_wall_tiles[i]);
        }

        // Draw player object
        window.draw(player_shape);
        window.display();
    }
}