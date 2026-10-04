#include "Player.h"

Player::Player()
{
    // Setup player transformation
    player_shape.setSize({ player_size, player_size });
    player_shape.setPosition({ 200.f, 450.f });
    player_shape.setOrigin(sf::Vector2f(player_size / 2.f, player_size / 2.f));
    
    // Error if can't load image
    if (!player_texture.loadFromFile("Sprites/Meatboy.png"))
    {
        throw "Error loading image";
    }

    player_shape.setTexture(&player_texture);
}

Player::~Player()
{
}

void Player::Jump(float _playerY_vel)
{
    player_shape.move({ 0.f, _playerY_vel });
}

float Player::UpdatePlayer(float _playerYvel, float _Yvelocity, float _dt)
{
    if (_playerYvel < 4.f) { _playerYvel += _Yvelocity * _dt; }
    return _playerYvel;
}
