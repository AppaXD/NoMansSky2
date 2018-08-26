#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Player
{
private:
	sf::Texture texture;
public:
	sf::Sprite sprite;
	int health;
	bool left = false;
	Player::Player(std::string texturePath);
	//Player::Player(std::string texturePath, Weapon playerWeapon);
};