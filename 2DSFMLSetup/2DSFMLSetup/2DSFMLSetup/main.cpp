#include <SFML/Graphics.hpp>
#include "Collisions.h"
#include "Level.h"
#include "Physics.h"
#include "Button.h"
#include "Player.h"
#include "Audio.h"
#include "ParticleSystem.h"

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

    // Background settings
    sf::Texture background_txt("background.jpg");
    sf::RectangleShape background({1280, 720 });
    background.setTexture(&background_txt);

    // Particle config
    // create the particle system
    ParticleSystem particles(1000);

    // create a clock to track the elapsed time
    sf::Clock clock;

    // Player config
    Player player;

    float playerY_vel = 0.f;
    float playerX_vel = 0.f;

    // Audio
    Audio jump_sound("jump.mp3");

    // Menu Buttons
    std::vector<Button> buttons;
    const int button_count = 3;

    // Text and font set up
    sf::Font Font1;
    if (!Font1.openFromFile("PressStart2P-Regular.ttf"))
    {
        throw "Font could not be loaded";
    }

    const int text_size = 16;

    // Create text for the buttons
    sf::Text PlayText(Font1, "Play", text_size);
    sf::Text CreditText(Font1, "Credit", text_size);
    sf::Text QuitText(Font1, "Quit", text_size);

    sf::Text button_roles[button_count] = {PlayText, CreditText, QuitText };

    // Create every menu button
    for (int i = 0; i < button_count; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({ 100.f * i, 650.f }, sf::Color::Green, button_roles[i]);
        buttons.push_back(NewButton);
        button_roles[i].setPosition({ 100.f * i + 10.f, 670.f });
        button_roles[i].setFillColor(sf::Color::Black);
    }

    bool gravity = true; // Change the direction / effect of gravity
    bool between_platform = false; // If the player is currently in between platforms

    // Load the first level
    Level MainLevel(15, 10);

    // While the game is running
    while (window.isOpen())
    {
        // Only do these events if the user has the window selected
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
            if (const auto* keyPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
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

                            // Credits
                            if (i == 1)
                            {
                                std::cout << "Game made by Zane";
                            }

                            // End game
                            if (i == 2)
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
        }

        // make the particle system emitter follow the mouse
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        particles.setEmitter(window.mapPixelToCoords(mouse));

        // update it
        sf::Time elapsed = clock.restart();
        particles.update(elapsed);

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
        }

        // Reversed gravtity
        else
        {
            playerY_vel = UpdatePlayer(playerY_vel, -1.f, 0.0198f);
        }

        player.Gravity(playerY_vel);

        // COLLISION PROCESSING - Y
        // Walls
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            // with Player
            if (player.GetPlayerShape().getGlobalBounds().findIntersection(MainLevel.level_wall_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveYCollisions(&player.player_shape, MainLevel.level_wall_tiles[i], false);
                playerY_vel = 0.f; // Set the players Y velocity to 0 if they are colliding with an object

                // VERTICAL MOVEMENT
                // Jump
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                {
                    std::cout << "\nPlayer: " << player.player_shape.getPosition().y;
                    std::cout << "\nWall: " << MainLevel.level_wall_tiles[i]->getPosition().y;

                    // Player can only jump off if they are on top of the block
                    if (player.player_shape.getPosition().y < MainLevel.level_wall_tiles[i]->getPosition().y)
                    {
                        playerY_vel = -2.5f;
                        jump_sound.Play();
                    }
                }
            }

            // with Box
            //if (MainLevel.level_box_tiles[i]->getGlobalBounds().findIntersection(MainLevel.level_wall_tiles[i]->getGlobalBounds()))
            //{
            //    // Collisions::ResolveYCollisions(MainLevel.level_box_tiles[i], MainLevel.level_wall_tiles[i], false);
            //}
        }
        
        // Platforms
        for (int i = 0; i < MainLevel.level_platform_tiles.size(); i++)
        {
            if (player.player_shape.getGlobalBounds().findIntersection(MainLevel.level_platform_tiles[i]->getGlobalBounds()))
            {
                // VERTICAL MOVEMENT
                // Jump
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                {
                    playerY_vel = -2.5f;
                    between_platform = true;
                }

                // Down through platform
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                {
                    playerY_vel = 2.5f;
                    between_platform = true;

                }

                // Only collide with platform if player isn't moving up or down and their velocity is 0
                if (!between_platform)
                {
                    playerY_vel = 0.f; // Set the players Y velocity to 0 if they are colliding with an object
                    Collisions::ResolveYCollisions(&player.player_shape, MainLevel.level_platform_tiles[i], false);
                }

                // If player stops coliding with the platform set 
            }
        }

        // Boxes
        for (int i = 0; i < MainLevel.level_box_tiles.size(); i++)
        {
            if (player.player_shape.getGlobalBounds().findIntersection(MainLevel.level_box_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveYCollisions(MainLevel.level_box_tiles[i], &player.player_shape, false);
            }
        }

        // Fall
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            playerY_vel = 5.0f;
        }

        // Drum
        for (int i = 0; i < MainLevel.level_drum_tiles.size(); i++)
        {
            if (player.GetPlayerShape().getGlobalBounds().findIntersection(MainLevel.level_drum_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveYCollisions(&player.player_shape, MainLevel.level_drum_tiles[i], false);

                // VERTICAL MOVEMENT
                playerY_vel = -5.0f;
                jump_sound.Play();
            }
        }

        // Spikes
        for (int i = 0; i < MainLevel.level_spike_tiles.size(); i++)
        {
            if (player.GetPlayerShape().getGlobalBounds().findIntersection(MainLevel.level_spike_tiles[i]->getGlobalBounds()))
            {
                player.ResetPosition();
            }
        }

        // Move on X
        player.Move(playerX_vel);

        // COLLISION PROCESSING - X
        // Player collides with WALL
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            // Stop the player
            if (player.GetPlayerShape().getGlobalBounds().findIntersection(MainLevel.level_wall_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveXCollisions(&player.player_shape, MainLevel.level_wall_tiles[i], false);
            }
        }

        // Player collides with BOX
        for (int i = 0; i < MainLevel.level_box_tiles.size(); i++)
        {
            // Move the box
            if (player.GetPlayerShape().getGlobalBounds().findIntersection(MainLevel.level_box_tiles[i]->getGlobalBounds()))
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                {
                    Collisions::ResolveXCollisions(MainLevel.level_box_tiles[i], &player.player_shape, false);
                }
            }
        }

        // Player collides with DRUM
        for (int i = 0; i < MainLevel.level_drum_tiles.size(); i++)
        {
            // Stop the player
            if (player.GetPlayerShape().getGlobalBounds().findIntersection(MainLevel.level_drum_tiles[i]->getGlobalBounds()))
            {
                Collisions::ResolveXCollisions(&player.player_shape, MainLevel.level_drum_tiles[i], false);
            }
        }

        window.clear();

        window.draw(background);

        // Draw all WALL tiles
        for (int i = 0; i < MainLevel.level_wall_tiles.size(); i++)
        {
            window.draw(*MainLevel.level_wall_tiles[i]);
        }

        // Draw all PLATFORM tiles
        for (int i = 0; i < MainLevel.level_platform_tiles.size(); i++)
        {
            window.draw(*MainLevel.level_platform_tiles[i]);
        }

        // Draw all BOX tiles
        for (int i = 0; i < MainLevel.level_box_tiles.size(); i++)
        {
            window.draw(*MainLevel.level_box_tiles[i]);
        }

        // Draw all DRUM tiles
        for (int i = 0; i < MainLevel.level_drum_tiles.size(); i++)
        {
            window.draw(*MainLevel.level_drum_tiles[i]);
        }

        // Draw all SPIKE tiles
        for (int i = 0; i < MainLevel.level_spike_tiles.size(); i++)
        {
            window.draw(*MainLevel.level_spike_tiles[i]);
        }

        // Draw menu buttons and text
        for (int i = 0; i < button_count; i++)
        {
            window.draw(buttons[i].m_ButtonShape);
            window.draw(button_roles[i]);
        }

        // Draw player object
        window.draw(player.player_shape);

        // Draw particles
        window.draw(particles);

        window.display();
    }
}