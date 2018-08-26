#include "Weapon.hpp"
#include <iostream>

Weapon::Weapon(bool isAutomatic, int weaponFirerate, int weaponDamage)
{
	automatic = isAutomatic;
	firerate = weaponFirerate;
	damage = weaponDamage;
}

void Weapon::startClock()
{
	clock.restart();
}

bool Weapon::canShoot()
{
	if (clock.getElapsedTime().asSeconds() >= 1.f / firerate)
	{
		clock.restart();
		return true;
	}

	return false;
}