#pragma once

#include <SFML/System/Clock.hpp>

class Weapon
{
public:
	bool automatic;
	int firerate, damage;
	sf::Clock clock;
	Weapon::Weapon(bool isAutomatic, int weaponFirerate, int weaponDamage);
	Weapon::Weapon() {};
	void startClock();
	bool canShoot();
};