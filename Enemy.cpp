#include "Enemy.hpp"

#include <SFML/Graphics/Texture.hpp>

Enemy::Enemy(std::string path, int firerate, int damage, int enemyHealth)
{
	texture.loadFromFile(path);
	sprite.setTexture(texture);

	sf::FloatRect floatRect = sprite.getGlobalBounds();

	sprite.setOrigin(floatRect.width / 2, floatRect.height / 2);

	weapon = Weapon(true, firerate, damage);
	health = enemyHealth;
}
