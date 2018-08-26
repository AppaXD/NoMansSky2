#include "Player.hpp"

Player::Player(std::string texturePath)
{
	texture.loadFromFile(texturePath);
	sprite.setTexture(texture);
	
	sf::FloatRect floatRect = sprite.getGlobalBounds();

	sprite.setOrigin(floatRect.width / 2, floatRect.height / 2);
}