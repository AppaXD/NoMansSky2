#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Weapon.hpp"
#include <string>

class Enemy
{
private:
	sf::Texture texture;
public:
	Weapon weapon;
	sf::Sprite sprite;
	int health;
	float acceleration = 150.f;
	bool deacceleration[2] = { false, false };
	float originalAngle;
	Enemy::Enemy(std::string path, int firerate, int damage, int enemyHealth);
};