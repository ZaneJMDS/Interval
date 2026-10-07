/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Interval game (2D platformer)
File name   :   Main.cpp
**************************************************************************/

#include <SFML/Graphics.hpp>
#include "Collisions.h"
#include "Level.h"
#include "Physics.h"
#include "Button.h"
#include "Audio.h"
#include "ParticleSystem.h"

int main()
{
    // Set window settings
    const int window_width = 1280;
    const int window_height = 720;
    sf::RenderWindow window(sf::VideoMode({ window_width, window_height }), "Interval");
    window.setFramerateLimit(60);
    sf::RenderWindow debug_window(sf::VideoMode({ 400, 400 }), "Debug Window");

    // Background settings
    sf::Texture background_txt("Sprites/background.png");
    sf::RectangleShape background({ window_width, window_height });
    background.setTexture(&background_txt);

    // create the particle system
    ParticleSystem particles(1000, sf::Color::Red);
    sf::Clock particle_clock;

    // Player config
    Player player;

    // Audio
    Audio menu_music("Audio/menu.mp3");

    // Menu Buttons
    std::vector<Button> buttons;
    const int button_count = 5;

    // Text and font set up
    sf::Font Font1;
    if (!Font1.openFromFile("PressStart2P-Regular.ttf"))
    {
        throw "Font could not be loaded";
    }

    const int text_size = 16;
    bool show_credits = false;

    // Title text
    sf::Text IntervalText(Font1, "Interval", 32);
    sf::Text CreditsText(Font1, "Game made by Zane J\nFont by cody@zone38.net\nSprites by Brackey\nBackground by Shackhal\nMusic by Antino & Wells", 16);
    IntervalText.setPosition({500.f, 100.f});
    CreditsText.setPosition({ 800.f, 300.f });

    sf::Text times_text(Font1, "", text_size);
    times_text.setPosition({ 140.f, 570.f });

    // Create text for the buttons
    sf::Text PlayText(Font1, "Play", text_size);
    sf::Text CreditText(Font1, "Credit", text_size);
    sf::Text QuitText(Font1, "Quit", text_size);
    sf::Text DecVolText(Font1, "Decrease", text_size);
    sf::Text IncVolText(Font1, "Increase", text_size);

    sf::Text button_roles[button_count] = {PlayText, CreditText, QuitText, DecVolText, IncVolText };

    sf::Text ResetButton(Font1, "Reset", text_size);

    // Create every menu button
    for (int i = 0; i < button_count; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({550.f, 100.f * i + 200.f }, sf::Color::Green, button_roles[i]);
        buttons.push_back(NewButton);
        button_roles[i].setPosition({ 560.f, 100.f * i + 210.f });
    }

    // Create debug buttons
    Button NewButton({ 50.f, 50.f}, sf::Color::Green, ResetButton);
    buttons.push_back(NewButton);
    ResetButton.setPosition({ 60.f, 60.f });

    // Load all levels here
    const int levels = 8;

    Level MainLevel("Levels/Level1.txt", Forest);
    Level SecondLevel("Levels/Level2.txt", Forest);
    Level ThirdLevel("Levels/Level3.txt", Desert);
    Level FourthLevel("Levels/Level4.txt", Desert);
    Level FifthLevel("Levels/level5.txt", Mountain);
    Level SixthLevel("Levels/level6.txt", Mountain);
    Level SeventhLevel("Levels/level7.txt", Snowy);
    Level EigthLevel("Levels/level8.txt", Snowy);

    int current_level = 0;

    Level Levels[levels] = { MainLevel, SecondLevel, ThirdLevel, FourthLevel, FifthLevel, SixthLevel, SeventhLevel, EigthLevel };

    bool gravity = true; // Change the direction / effect of gravity
    bool between_platform = false; // If the player is currently in between platforms
    bool game_start = false; // If the player is in the main menu or not
    bool game_completion = false; // If the player has beat the final level

    menu_music.Play(); // Start playing the music

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

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        player.Animate();

        // Update stopwatch and display to screen
        Levels[current_level].StopwatchUpdate();

        // Check the player hasn't left the map
        if (player.player_shape.getPosition().x > window_width || player.player_shape.getPosition().x < -50 || player.player_shape.getPosition().y > window_height || player.player_shape.getPosition().y < -50) { Levels[current_level].Reset(&player); }

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

                // Music volume controls
                if (keyPressed->code == sf::Keyboard::Key::O)
                {
                    menu_music.DecreaseVolume();
                }

                if (keyPressed->code == sf::Keyboard::Key::P)
                {
                    menu_music.IncreaseVolume();
                }

                // Dev Debug Level switcher
                for (int i = 0; i < levels; i++)
                {
                    // Dont unload the level if it is current one 
                    if (i != current_level)
                    {
                        // Ascii values for number keys
                        if (keyPressed->code == sf::Keyboard::Key(i + 27))
                        {
                            Levels[current_level].UnloadLevel();
                            current_level = i;
                            Levels[current_level].LoadLevel();
                            Levels[current_level].Reset(&player);
                        }
                    }
                }
            }

            // Interactable menu if the game hasn't started
            if (!game_start)
            {
                if (const auto* keyPressed = event->getIf<sf::Event::MouseButtonPressed>())
                {
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
                                    current_level = 0;
                                    game_start = true;
                                    Levels[current_level].LoadLevel();
                                    Levels[current_level].Reset(&player);
                                }

                                // Credits
                                if (i == 1)
                                {
                                    show_credits = !show_credits;
                                }

                                // End game
                                if (i == 2)
                                {
                                    return 0;
                                }

                                // Decrease vol
                                if (i == 3)
                                {
                                    menu_music.DecreaseVolume();
                                }

                                // Increase vol
                                if (i == 4)
                                {
                                    menu_music.IncreaseVolume();
                                }
                            }
                        }
                    }
                }
            }
        }

        // Debug window events
        while (const std::optional event = debug_window.pollEvent())
        {
            if (const auto* keyPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // Left MB pressed
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    // Menu
                    if (NewButton.m_ButtonShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(debug_window))))
                    {
                        Levels[current_level].Reset(&player);
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
            // update particles
            sf::Time elapsed = particle_clock.getElapsedTime();
            particles.update(elapsed);

            // HORIZONTAL MOVEMENT
            // Left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                if (player.Xvelocity > -5.f)
                {
                    player.Xvelocity -= 0.1f;
                }

                else 
                { 
                    // update particles
                    particle_clock.restart();
                    particles.setEmitter(player.player_shape.getPosition());
                }

            }

            // Right
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                if (player.Xvelocity < 5.f)
                {
                    player.Xvelocity += 0.1f;
                }
                
                else 
                { 
                    // update particles
                    particle_clock.restart();
                    particles.setEmitter(player.player_shape.getPosition());
                }
            }


            // Reset player's X velocity if they stop moving left and right
            else
            {
                if (player.Xvelocity < 0.2f && player.Xvelocity > -0.2f) { player.Xvelocity = 0.f; }
                if (player.Xvelocity > 0.f) { player.Xvelocity -= 0.2f;}
                if (player.Xvelocity < 0.f) { player.Xvelocity += 0.2f;}
            }

            // Normal gravity
            if (gravity)
            {
                player.UpdatePlayer(1.f, 0.02f);
            }

            // Reversed gravtity
            else
            {
                player.UpdatePlayer(-1.f, 0.02f);
            }

            player.Gravity();

            // COLLISION PROCESSING - Y
            // Walls
            for (int i = 0; i < Levels[current_level].level_wall_tiles.size(); i++)
            {
                // with Player
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_wall_tiles[i]->getGlobalBounds()))
                {
                    Collisions::ResolveYCollisions(&player.player_shape, Levels[current_level].level_wall_tiles[i], false);
                    player.Yvelocity = 0.f; // Set the players Y velocity to 0 if they are colliding with an object

                    // VERTICAL MOVEMENT
                    // Jump
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    {
                        // Player can only jump off if they are on top of the block when grvaity is true
                        if (player.player_shape.getPosition().y < Levels[current_level].level_wall_tiles[i]->getPosition().y && gravity)
                        {
                            player.Yvelocity = -3.f;
                            player.Jump();
                        }

                        // 
                        if (player.player_shape.getPosition().y > Levels[current_level].level_wall_tiles[i]->getPosition().y && !gravity)
                        {
                            player.Yvelocity = 3.f;
                            player.Jump();
                        }
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
                        player.Yvelocity = -3.f;
                        player.Jump();
                    }

                    // Check if player is currently below platform
                    if (player.Yvelocity < 0 && gravity) { between_platform = true; }
                    else if (player.Yvelocity > 0 && !gravity) { between_platform = true; }

                    // Down through platform
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                    {
                        between_platform = true;
                    }

                    // Only collide with platform if player isn't moving up or down and their velocity is 0
                    if (!between_platform)
                    {
                        player.Yvelocity = 0.f; // Set the players Y velocity to 0 if they are colliding with an object
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
                    // Stop the player if they are above the box
                    if (player.player_shape.getPosition().y < Levels[current_level].level_box_tiles[i]->getPosition().y)
                    {
                        Collisions::ResolveYCollisions(&player.player_shape, Levels[current_level].level_box_tiles[i], false);
                    }

                    // Jump
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    {
                        player.Yvelocity = -3.f;
                        player.Jump();
                    }
                }
            }

            // Fall
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                player.Yvelocity = 4.0f;
            }

            // Drum
            for (int i = 0; i < Levels[current_level].level_drum_tiles.size(); i++)
            {
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_drum_tiles[i]->getGlobalBounds()))
                {
                    Collisions::ResolveYCollisions(&player.player_shape, Levels[current_level].level_drum_tiles[i], false);
                    player.Yvelocity = -4.0f;
                    player.Jump();
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
            std::string level_times = "";

            for (int i = 0; i < Levels[current_level].level_goal_tile.size(); i++)
            {
                if (player.player_shape.getGlobalBounds().findIntersection(Levels[current_level].level_goal_tile[i]->getGlobalBounds()))
                {
                    Levels[current_level].UnloadLevel();
                    
                    // If there are still remaining levels
                    if (current_level < levels - 1)
                    {
                        current_level++;
                        Levels[current_level].LoadLevel();
                        Levels[current_level].Reset(&player);
                    }

                    // This was the last level
                    else
                    {
                        game_completion = true;
                        level_times = "Final times:\n";

                        // Display scores to the user
                        for (int i = 0; i < levels; i++)
                        {
                            level_times += "Level " + std::to_string(i + 1) + ": " + std::to_string(Levels[i].GetTime()) + "s\n";
                        }

                        times_text.setString(level_times);
                        game_start = false;
                    }
                }
            }

            // Move on X
            player.Move();

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
                    if (player.Xvelocity != 0.f)
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
        debug_window.clear();
        
        debug_window.draw(NewButton.m_ButtonShape);
        debug_window.draw(ResetButton);

        // Draw the game
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

            // Draw player's time and level
            window.draw(Levels[current_level].StopwatchUpdate());
            sf::Text level_text(Font1, "Level: " + std::to_string(current_level + 1));
            level_text.setPosition({500.f, 650.f});
            window.draw(level_text);

            // Draw particles
            window.draw(particles);
        }

        // Menu
        else
        {
            window.draw(IntervalText);

            // Draw menu buttons and text
            for (int i = 0; i < button_count; i++)
            {
                window.draw(buttons[i].m_ButtonShape);
                window.draw(button_roles[i]);
            }

            // Volume to screen
            sf::Text volume_text(Font1, "Volume: " + std::to_string(int(menu_music.GetSound().getVolume())), text_size); // Big cast from float, to int, to string, to text
            volume_text.setPosition({ 540.f, 570.f});
            window.draw(volume_text);

            // Show credits if buttons were clicked
            if (show_credits) { window.draw(CreditsText); }
            window.draw(times_text);
            
            sf::Text completion_text(Font1, "Game completed: false", text_size);
            completion_text.setPosition({ 100.f, 220.f });
            if (game_completion) { completion_text.setString("Game completed: true"); }
            window.draw(completion_text);
        }

        window.display();
        debug_window.display();
    }
}