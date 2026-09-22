#include "Player.h"

Player::Player()
{
    // Setup player transformation
    player_shape.setSize({ player_size, player_size });
    player_shape.setPosition({ 200.f, 450.f });
    player_shape.setOrigin(sf::Vector2f(player_size / 2.f, player_size / 2.f));
    
    // Error if can't load image
    if (!player_texture.loadFromFile("Meatboy.png"))
    {
        throw "Error loading image";
    }

    player_shape.setTexture(&player_texture);
}

Player::~Player()
{
}
