#include <SFML/Graphics.hpp>
#include "Collisions.h"
#include "Level.h"
#include "Physics.h"
#include "Button.h"
#include "Player.h"
#include "Audio.h"
#include "ParticleSystem.h"
#include "Enemy.h"

int main()
{
    // Set window settings
    const int window_width = 1280;
    const int window_height = 720;
    sf::RenderWindow window(sf::VideoMode({ window_width, window_height }), "Interval");
    window.setFramerateLimit(60);

    // Background settings
    sf::Texture background_txt("Sprites/background.png");
    sf::RectangleShape background({ window_width, window_height });
    background.setTexture(&background_txt);

    // Particle config
    // create the particle system
    ParticleSystem particles(1000, sf::Color::Red);

    // create a clock for particles
    sf::Clock particle_clock;

    // Create a clock for player time
    sf::Clock stopwatch;

    // Player config
    Player player;

    float playerY_vel = 0.f;
    float playerX_vel = 0.f;

    // Audio
    Audio jump_sound("Audio/jump.mp3");
    Audio menu_music("Audio/menu.mp3");

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

    // Title text
    sf::Text IntervalText(Font1, "Interval", 32);
    IntervalText.setPosition({500.f, 100.f});

    // Create text for the buttons
    sf::Text PlayText(Font1, "Play", text_size);
    sf::Text CreditText(Font1, "Credit", text_size);
    sf::Text QuitText(Font1, "Quit", text_size);

    sf::Text button_roles[button_count] = {PlayText, CreditText, QuitText };

    // Create every menu button
    for (int i = 0; i < button_count; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({550.f, 100.f * i + 200.f }, sf::Color::Green, button_roles[i]);
        buttons.push_back(NewButton);
        button_roles[i].setPosition({ 560.f, 100.f * i + 210.f });
        button_roles[i].setFillColor(sf::Color::Black);
    }

    // Load all levels
    const int levels = 4;

    Level MainLevel("Levels/Level1.txt", Forest);
    Level SecondLevel("Levels/Level2.txt", Desert);
    Level ThirdLevel("Levels/Level3.txt", Mountain);
    Level FourthLevel("Levels/Level4.txt", Snowy);

    int current_level = 0;
    MainLevel.LoadLevel();

    Level Levels[levels] = { MainLevel, SecondLevel, ThirdLevel, FourthLevel };

    bool gravity = true; // Change the direction / effect of gravity
    bool between_platform = false; // If the player is currently in between platforms
    bool game_start = false; // If the player is in the main menu or not

    menu_music.Play();

    // While the game is running
    while (window.isOpen())
    {
        // Enemy logic
        for (int i = 0; i < Levels[current_level].level_enemies.size(); i++)
        {
            Levels[current_level].level_enemies[i].Animate();
            Levels[current_level].level_enemies[i].Move();

            // If Enemy and player collide
            if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_enemies[i].enemy_shape.getGlobalBounds()))
            {
                Levels[current_level].Reset(&player);
            }

            // Check the enemy hasn't left the map
            if (Levels[current_level].level_enemies[i].enemy_shape.getPosition().x > window_width || Levels[current_level].level_enemies[i].enemy_shape.getPosition().x < 0 || Levels[current_level].level_enemies[i].enemy_shape.getPosition().y > window_height || Levels[current_level].level_enemies[i].enemy_shape.getPosition().y < 0) { Levels[current_level].level_enemies[i].Reset(); }
        }

        // make the particle system emitter follow the mouse
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        // particles.setEmitter(window.mapPixelToCoords(mouse_pos));

        // update partciles using the mouse
        sf::Time elapsed = particle_clock.restart();
        particles.update(elapsed);

        // Update stopwatch
        sf::Time elapsed2 = stopwatch.getElapsedTime();
        int current_time = (elapsed2.asSeconds());
        sf::Text ClockText(Font1, "TIME: " + std::to_string(current_time), 32);
        ClockText.setPosition({ 150.f, 650.f });

        // Check the player hasn't left the map
        if (player.player_shape.getPosition().x > window_width || player.player_shape.getPosition().x < 0 || player.player_shape.getPosition().y > window_height || player.player_shape.getPosition().y < 0) { player.ResetPosition(); }

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

            // Keybinds that the player should click, not hold
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // Swap gravity direction
                if (keyPressed->code == sf::Keyboard::Key::G)
                {
                    gravity = !gravity;
                    player.Rotate();
                }

                // Reset level
                if (keyPressed->code == sf::Keyboard::Key::R)
                {
                    Levels[current_level].Reset(&player);
                }
            }

            // Interactable menu if the game hasn't started
            if (!game_start)
            {
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
                                    game_start = true;
                                    stopwatch.restart();
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
        }

        // Did user hover on a button
        for (int i = 0; i < button_count; i++)
        {
            if (buttons[i].m_ButtonShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
            {
                buttons[i].m_ButtonShape.setOutlineThickness(-8.f);
            }

            else
            {
                buttons[i].m_ButtonShape.setOutlineThickness(-2.f);
            }
        }

        // In game events
        if (game_start)
        {
            // menu_music.sound.stop();

            // HORIZONTAL MOVEMENT
            // Left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                if (playerX_vel > -5.f)
                {
                    playerX_vel -= 0.1f;
                }

                else { particles.setEmitter(player.player_shape.getPosition()); }

            }

            // Right
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                if (playerX_vel < 5.f)
                {
                    playerX_vel += 0.1f;
                }
                
                else { particles.setEmitter(player.player_shape.getPosition()); }
            }

            // Reset player's X velocity if they stop moving left and right
            else
            {
                if (playerX_vel < 0.2f && playerX_vel > -0.2f) { playerX_vel = 0.f; }
                if (playerX_vel > 0.f) { playerX_vel -= 0.2f;}
                if (playerX_vel < 0.f) { playerX_vel += 0.2f;}
            }

            // Normal gravity
            if (gravity)
            {
                playerY_vel = player.UpdatePlayer(playerY_vel, 1.f, 0.0198f);
            }

            // Reversed gravtity
            else
            {
                playerY_vel = player.UpdatePlayer(playerY_vel, -1.f, 0.0198f);
            }

            player.Gravity(playerY_vel);

            // COLLISION PROCESSING - Y
            // Walls
            for (int i = 0; i < Levels[current_level].level_wall_tiles.size(); i++)
            {
                // with Player
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_wall_tiles[i]->getGlobalBounds()))
                {
                    Collisions::ResolveYCollisions(&player.player_shape, Levels[current_level].level_wall_tiles[i], false);
                    playerY_vel = 0.f; // Set the players Y velocity to 0 if they are colliding with an object

                    // VERTICAL MOVEMENT
                    // Jump
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    {
                        // Player can only jump off if they are on top of the block when grvaity is true
                        if (player.player_shape.getPosition().y < Levels[current_level].level_wall_tiles[i]->getPosition().y && gravity)
                        {
                            playerY_vel = -3.f;
                            jump_sound.GetSound().play();
                        }

                        // 
                        if (player.player_shape.getPosition().y > Levels[current_level].level_wall_tiles[i]->getPosition().y && !gravity)
                        {
                            playerY_vel = 3.f;
                            jump_sound.GetSound().play();
                        }
                    }
                }

                // with BOX
                for (int j = 0; j < Levels[current_level].level_box_tiles.size(); j++)
                {
                    // Move the BOX
                    if (Levels[current_level].level_box_tiles[j]->getGlobalBounds().findIntersection(Levels[current_level].level_wall_tiles[i]->getGlobalBounds()))
                    {
                        Collisions::ResolveYCollisions(Levels[current_level].level_box_tiles[j], Levels[current_level].level_wall_tiles[i], false);
                    }
                }
            }

            int platforms_collided = 0; // Counter to see if the player isn't colliding with any platform

            // Platforms
            for (int i = 0; i < Levels[current_level].level_platform_tiles.size(); i++)
            {
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_platform_tiles[i]->getGlobalBounds()))
                {
                    // VERTICAL MOVEMENT
                    // Jump
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    {
                        playerY_vel = -3.f;
                    }

                    // Check if player is currently below platform
                    if (playerY_vel < 0 && gravity) { between_platform = true; }
                    else if (playerY_vel > 0 && !gravity) { between_platform = true; }

                    // Down through platform
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                    {
                        playerY_vel = 3.f;
                        between_platform = true;
                    }

                    // Only collide with platform if player isn't moving up or down and their velocity is 0
                    if (!between_platform)
                    {
                        playerY_vel = 0.f; // Set the players Y velocity to 0 if they are colliding with an object
                        Collisions::ResolveYCollisions(&player.player_shape, Levels[current_level].level_platform_tiles[i], false);
                    }
                }

                // If player stops coliding with the platform set 
                if (!player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_platform_tiles[i]->getGlobalBounds()))
                {
                    platforms_collided++;
                    if (platforms_collided == Levels[current_level].level_platform_tiles.size()) { between_platform = false; }
                }
            }

            // Boxes
            for (int i = 0; i < Levels[current_level].level_box_tiles.size(); i++)
            {
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_box_tiles[i]->getGlobalBounds()))
                {
                    // Swap the collisions around to move the box instead of the player
                    Collisions::ResolveYCollisions(Levels[current_level].level_box_tiles[i], &player.player_shape, false);
                }
            }

            // Fall
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                playerY_vel = 5.0f;
            }

            // Drum
            for (int i = 0; i < Levels[current_level].level_drum_tiles.size(); i++)
            {
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_drum_tiles[i]->getGlobalBounds()))
                {
                    Collisions::ResolveYCollisions(&player.player_shape, Levels[current_level].level_drum_tiles[i], false);

                    // VERTICAL MOVEMENT
                    playerY_vel = -5.0f;
                    jump_sound.GetSound().play();
                }
            }

            // Spikes
            for (int i = 0; i < Levels[current_level].level_spike_tiles.size(); i++)
            {
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_spike_tiles[i]->getGlobalBounds()))
                {
                    Levels[current_level].Reset(&player);
                }
            }

            // Goal
            for (int i = 0; i < Levels[current_level].level_goal_tile.size(); i++)
            {
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_goal_tile[i]->getGlobalBounds()))
                {
                    Levels[current_level].UnloadLevel();
                    
                    // If there are still remaining levels
                    if (current_level < levels)
                    {
                        current_level++;
                        Levels[current_level].LoadLevel();
                        Levels[current_level].Reset(&player);
                        stopwatch.restart();
                    }

                    // This was the last level
                    else
                    {
                        game_start = false;
                    }
                }
            }

            // Move on X
            player.Move(playerX_vel);

            // COLLISION PROCESSING - X
            // WALL collides with
            for (int i = 0; i < Levels[current_level].level_wall_tiles.size(); i++)
            {
                // PLAYER
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_wall_tiles[i]->getGlobalBounds()))
                {
                    Collisions::ResolveXCollisions(&player.player_shape, Levels[current_level].level_wall_tiles[i], false);
                }

                // ENEMY
                for (int j = 0; j < Levels[current_level].level_enemies.size(); j++)
                {
                    if (Levels[current_level].level_enemies[j].enemy_shape.getGlobalBounds().findIntersection(Levels[current_level].level_wall_tiles[i]->getGlobalBounds()))
                    {
                        Collisions::ResolveXCollisions(&Levels[current_level].level_enemies[j].enemy_shape, Levels[current_level].level_wall_tiles[i], false);
                        Levels[current_level].level_enemies[j].Rotate();
                    }
                }

                // BOX
                for (int j = 0; j < Levels[current_level].level_box_tiles.size(); j++)
                {
                    if (Levels[current_level].level_box_tiles[j]->getGlobalBounds().findIntersection(Levels[current_level].level_wall_tiles[i]->getGlobalBounds()))
                    {
                        Collisions::ResolveXCollisions(Levels[current_level].level_box_tiles[j], Levels[current_level].level_wall_tiles[i], false);
                    }
                }
            }

            // Player collides with BOX
            for (int i = 0; i < Levels[current_level].level_box_tiles.size(); i++)
            {
                // Move the box
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_box_tiles[i]->getGlobalBounds()))
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    {
                        Collisions::ResolveXCollisions(Levels[current_level].level_box_tiles[i], &player.player_shape, false);
                    }
                }
            }

            // Player collides with DRUM
            for (int i = 0; i < Levels[current_level].level_drum_tiles.size(); i++)
            {
                // Stop the player
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_drum_tiles[i]->getGlobalBounds()))
                {
                    Collisions::ResolveXCollisions(&player.player_shape, Levels[current_level].level_drum_tiles[i], false);
                }
            }
        }

        window.clear();

        if (game_start)
        {
            window.draw(background);

            // Draw all level tiles
            for (int i = 0; i < Levels[current_level].level_tiles.size(); i++)
            {
                // Draw all specific tile type
                for (int j = 0; j < Levels[current_level].level_tiles[i].size(); j++)
                {
                    window.draw(*Levels[current_level].level_tiles[i][j]);
                }
            }

            // Draw all enemies
            for (int i = 0; i < Levels[current_level].level_enemies.size(); i++)
            {
                window.draw(Levels[current_level].level_enemies[i].enemy_shape);
            }

            // Draw player object
            window.draw(player.player_shape);

            // Draw player's time
            window.draw(ClockText);
        }

        else
        {
            window.draw(IntervalText);

            // Draw menu buttons and text
            for (int i = 0; i < button_count; i++)
            {
                window.draw(buttons[i].m_ButtonShape);
                window.draw(button_roles[i]);
            }
        }

        // Draw particles
        window.draw(particles);

        window.display();
    }
}