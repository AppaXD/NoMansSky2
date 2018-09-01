#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Player
{
private:
	sf::Texture texture;
public:
	sf::Sprite sprite;
	short health = 100;
	bool left = false;
	Player::Player(std::string texturePath);
	//Player::Player(std::string texturePath, Weapon playerWeapon);
};