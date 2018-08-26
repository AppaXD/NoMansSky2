#include "Entity.hpp"
Entity::Entity(sf::Sprite entitySprite, int entitySource, int entityDamage)
{
	sprite = entitySprite;
	source = entitySource;
	damage = entityDamage;
}

void Entity::move(float speed, float delta)
{
	float angle = sprite.getRotation() - 90;
	float pi = 3.14159265;

	sprite.move(delta * speed * cos(angle * pi / 180), delta * speed* sin(angle * pi / 180));
}
