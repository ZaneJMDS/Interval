#include "Player.h"

Player::Player()
{
    // Setup player transformation
    player_shape.setSize({ player_size, player_size });
    player_shape.setPosition({ 100.f, 500.f });
    player_shape.setOrigin(sf::Vector2f(player_size / 2.f, player_size / 2.f));
    
    // Error if can't load image
    if (!player_texture.loadFromFile("Sprites/player.png"))
    {
        throw "Error loading image";
    }

    // Location and size of texture
    player_shape.setTextureRect(sf::IntRect({ 0, 0 }, { 24, 24 }));
    player_shape.setTexture(&player_texture);
}

Player::~Player()
{
}

// Reset the world, this includes the player, boxes, and enemies
void Player::ResetPosition()
{
    // Reset the player
    player_shape.setPosition({ 100.f, 500.f });
}

void Player::Jump()
{
    Audio jump_sound("Audio/jump.mp3");
    jump_sound.Play();
    // player_shape.move({ 0.f, _playerY_vel });
}

void Player::Animate()
{
    // How many seconds have passed
    sf::Time elapsed2 = animation_clock.getElapsedTime();
    float current_time = (elapsed2.asSeconds());

    // Location and size of texture
    // FRAMES
    if (current_time < 0.5f) { player_shape.setTextureRect(sf::IntRect({ 0, 0 }, { 24, 24 })); }
    else if (current_time < 1.f) { player_shape.setTextureRect(sf::IntRect({ 24, 0 }, { 24, 24 })); }
    else { animation_clock.restart(); }
}

// Update player's position relative to gravity
void Player::UpdatePlayer(float _Yvelocity, float _dt)
{
    if (Yvelocity < 4.f) { Yvelocity += _Yvelocity * _dt; }
}
