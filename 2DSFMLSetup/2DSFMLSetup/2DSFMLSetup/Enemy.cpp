#include "Enemy.h"

Enemy::Enemy(sf::Vector2f _start_pos, sf::Texture _enemy_texture)
{
    start_pos = _start_pos;
    enemy_texture2 = _enemy_texture;

    // Setup player transformation
    enemy_shape.setSize({ enemy_size, enemy_size });
    enemy_shape.setOrigin(sf::Vector2f(enemy_size / 2.f, enemy_size / 2.f));

    Reset();

    enemy_shape.setFillColor(sf::Color::Green);

    enemy_shape.setTextureRect(sf::IntRect({ 4, 32 }, { 16, 16 })); // Starting frame
    // enemy_shape.setTexture(&enemy_texture2);
}

Enemy::~Enemy()
{
}

// Go through the default animations
void Enemy::Animate()
{
    // How many seconds have passed
    sf::Time elapsed2 = animation_clock.getElapsedTime();
    float current_time = (elapsed2.asSeconds());

    // Location and size of texture
    // FRAMES
    if (current_time < 0.25f) enemy_shape.setTextureRect(sf::IntRect({ 28, 32 }, { 16, 16 }));
    else if (current_time < 0.5f) enemy_shape.setTextureRect(sf::IntRect({ 52, 32 }, { 16, 16 }));
    else if (current_time < 0.75f) enemy_shape.setTextureRect(sf::IntRect({ 76, 32 }, { 16, 16 }));
    else if (current_time < 1.f) { enemy_shape.setTextureRect(sf::IntRect({ 4, 32 }, { 16, 16 })); }
    else if (current_time > 1.f) { animation_clock.restart(); } // Start from the beginning
}

// Reset enemy position
void Enemy::Reset()
{
    enemy_shape.setPosition(start_pos);
    enemy_shape.setScale({ -1.0f, 1.0f });
}

void Enemy::Rotate()
{
    if (enemy_shape.getScale() == sf::Vector2f{ 1.f, 1.f }) { enemy_shape.setScale({ -1.0f, 1.0f }); } // Face left
    else { enemy_shape.setScale({ 1.0f, 1.0f }); } // Face right
}

void Enemy::Move()
{
    if (enemy_shape.getScale() == sf::Vector2f{ -1.f, 1.f }) { enemy_shape.move({-1.f, 0.f}); } // Move left
    else { enemy_shape.move({1.f, 0.f}); } // Move right
}

