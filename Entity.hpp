#pragma once

#include <SFML/Graphics/Sprite.hpp>

class Entity
{
public:
	sf::Sprite sprite;
	int source; // 0 for player, 1 for enemy
	int damage;
	Entity::Entity(sf::Sprite entitySprite, int entitySource, int entityDamage);
	void move(float speed, float delta);
};