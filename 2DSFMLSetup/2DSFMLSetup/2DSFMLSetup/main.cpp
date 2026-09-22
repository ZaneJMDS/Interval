#include <SFML/Graphics.hpp>
#include "Collisions.h"
#include "Level.h"
#include "Physics.h"
#include "Button.h"
#include "Player.h"

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

    Player player;

    // Menu Buttons
    std::vector<Button> buttons;

    const int button_count = 2;

    for (int i = 0; i < button_count; i++)
    {
        // Makes button with role, location, and colour 
        Button NewButton({ 100.f * i, 600.f }, sf::Color::Green);
        buttons.push_back(NewButton);
    }

    bool gravity = true; // Change the direction of gravity

    float playerY_vel = 0.f;
    float playerX_vel = 0.f;

    // Load the first level
    Level MainLevel(15, 10);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // If close button pressed closed the window
            if (event->is<sf::Event::Closed>()) { window.close(); }

            // Catch the resize events
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea({ 0.f, 0.f }, sf::Vector2f(resized->size));
                window.setView(sf::View(visibleArea));
            }

            // Swap gravity direction
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::G)
                {
                    gravity = !gravity;
                    player.Rotate();
                }
            }

            // Left MB pressed
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                // Did user click on a button
                for (int i = 0; i < button_count; i++)
                {
                    // Menu
                    if (buttons[i].m_ButtonShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
                    {
                        // Start game
                        if (i == 0)
                        {
                            
                        }

                        // Exit
                        if (i == 1)
                        {
                            return 0;
                        }
                    }
                }
            }
        }

        playerX_vel = 0.f; // Reset player's X velocity if they stop moving left and right

        // KEYBINDS
        // Reset player
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {
            player.ResetPosition();
        }

        // HORIZONTAL MOVEMENT
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            playerX_vel = -5.f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            playerX_vel = 5.f;
        }

        // Normal gravity
        if (gravity)
        {
            playerY_vel = UpdatePlayer(playerY_vel, 1.f, 0.0198f);
            player.Gravity(playerY_vel);
        }

        // Reversed gravtity
        else
        {
            playerY_vel = UpdatePlayer(playerY_vel, -1.f, 0.0198f);
            player.Gravity(playerY_vel);
        }

        // COLLISION PROCESSING - Y
        // Walls
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            if (player.GetPlayerShape().getGlobalBounds().findIntersection(MainLevel.level_wall_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveYCollisions(&player.player_shape, MainLevel.level_wall_tiles[i], false);
                playerY_vel = 0.f; // Set the players Y velocity to 0 if they are colliding with an object

                // VERTICAL MOVEMENT
                // Jump
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                {
                    playerY_vel = -2.5f;
                }
            }
        }
        // Platforms
        for (int i = 0; i < MainLevel.level_platform_tiles.size(); i++)
        {
            // CRASHES 
            if (player.player_shape.getGlobalBounds().findIntersection(MainLevel.level_platform_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveYCollisions(&player.player_shape, MainLevel.level_platform_tiles[i], false);
                playerY_vel = 0.f; // Set the players Y velocity to 0 if they are colliding with an object

                // VERTICAL MOVEMENT
                // Fall through
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                {
                    playerY_vel = 5.f;
                }
            }
        }

        // Move on X
        player.Move(playerX_vel);

        // COLLISION PROCESSING - X
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            if (player.GetPlayerShape().getGlobalBounds().findIntersection(MainLevel.level_wall_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveXCollisions(&player.player_shape, MainLevel.level_wall_tiles[i], false);
            }
        }

        window.clear();

        // Draw all the level tiles
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            window.draw(*MainLevel.level_platform_tiles[i]);
        }

        // Draw all level platform tiles
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            window.draw(*MainLevel.level_platform_tiles[i]);
        }

        // Draw menu buttons
        for (int i = 0; i < button_count; i++)
        {
            window.draw(buttons[i].m_ButtonShape);
        }

        // Draw player object
        window.draw(player.player_shape);
        window.display();
    }
}