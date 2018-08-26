#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "Player.hpp"
#include "Weapon.hpp"
#include "Entity.hpp"
#include "Collision.hpp" // not by me! obb collision from https://github.com/SFML/SFML/wiki/Source%3A-Simple-Collision-Detection-for-SFML-2
#include "Enemy.hpp"

std::string assets = "assets/";

std::vector<sf::Texture> textures;
sf::Texture texture;

float pi = 3.14159265;

sf::Vector2f getCentre(sf::Vector2f size)
{
	return sf::Vector2f(size.x / 2, size.y / 2);
}

sf::Vector2f getCentre(sf::FloatRect floatRect)
{
	return sf::Vector2f(floatRect.width / 2, floatRect.height / 2);
}

sf::Sprite createSprite(sf::Texture& texture, bool centre)
{
	sf::Sprite sprite;
	sprite.setTexture(texture);
	if(centre) sprite.setOrigin(getCentre(sprite.getGlobalBounds()));

	return sprite;
}

float rotateTowards(sf::Vector2f pos1, sf::Vector2f pos2)
{
	float dx = pos1.x - pos2.x;
	float dy = pos1.y - pos2.y;

	float angle = ((atan2(dy, dx)) * 180 / pi);

	return angle;
}

int main()
{
	int width = 1600;
	int height = 900;

	std::string l;
	std::ifstream settings("settings.txt");
	std::getline(settings, l);

	if (l.empty()) l = "0";

	int aaLevel = std::stoi(l);

	if (aaLevel > 16) aaLevel = 16;
	if (aaLevel < 0) aaLevel = 0;

	std::cout << "Setting anti-aliasing level to " << aaLevel << ". This value can be changed in settings.txt\n";

	sf::ContextSettings windowSettings;
	windowSettings.antialiasingLevel = aaLevel;

	sf::RenderWindow window(sf::VideoMode(width, height), "Spacegame", sf::Style::Default, windowSettings);
	window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);

	sf::View view(sf::FloatRect(0, 0, width, height));

	sf::Texture t_crosshair;
	t_crosshair.loadFromFile(assets + "crosshair.png");
	sf::Sprite crosshair = createSprite(t_crosshair, true);

	sf::Texture t_redLaser;
	t_redLaser.loadFromFile(assets + "redLaser.png");
	sf::Sprite redLaser = createSprite(t_redLaser, true);
	redLaser.setScale(1.f, 1.5f);

	Player player(assets + "ship.png");
	player.sprite.setPosition(width / 2, height / 2);

	Enemy enemy(assets + "enemyBlack.png", 2, 6, 40);
	enemy.sprite.setPosition(500, 200);

	sf::Vector2f p1_offset(10, -28);
	sf::RectangleShape point1(sf::Vector2f(3, 3));
	point1.setFillColor(sf::Color::Green);
	point1.setOrigin(p1_offset);
	point1.setPosition(player.sprite.getPosition());

	sf::Vector2f p2_offset(10, 28);
	sf::RectangleShape point2(sf::Vector2f(3, 3));
	point2.setFillColor(sf::Color::Green);
	point2.setOrigin(p2_offset);
	point2.setPosition(player.sprite.getPosition());

	Weapon weapon(true, 5, 5);

	std::vector<Enemy> enemies;
	enemies.push_back(enemy);

	sf::Vector2f acceleration;

	bool direction[4] = { 
		false, // up
		false, // down
		false, // left
		false  // right
	};

	bool deacceleration[4] = { false, false, false, false };

	sf::Clock clock;
	float delta;

	std::vector<Entity> lasers;
	sf::Vector2f ppos(0, 0);
	float angle;
	sf::Vector2f mousePosition;

	bool lmbDown = false;

	weapon.startClock();

	while (window.isOpen())
	{
		delta = clock.restart().asSeconds();

		angle = rotateTowards(player.sprite.getPosition(), mousePosition);
		ppos = player.sprite.getPosition();
		mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
			{
				bool up = event.type == sf::Event::KeyReleased;

				int key = event.key.code;

				switch (key)
				{
					case sf::Keyboard::W:
						if (!direction[1] && !deacceleration[1])
						{
							direction[0] = true;
							if (acceleration.y != -40 && acceleration.y > -40) acceleration.y = -40;
						}
						if (up)
						{
							direction[0] = false;
						}
						break;
					case sf::Keyboard::S:
						if (!direction[0] && !deacceleration[0])
						{
							direction[1] = true;
							if(acceleration.y != 40 && acceleration.y < 40) acceleration.y = 40;
						}
						if (up)
						{
							direction[1] = false;
						}
						break;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (weapon.automatic)
				{
					lmbDown = true;
				}
				else if (weapon.canShoot())
				{
					sf::Vector2f bulletStart(0, 0);
					if (player.left)
					{
						player.left = false;
						bulletStart.x = ppos.x + p1_offset.x * std::cos(angle * pi / 180) - p1_offset.y * std::sin(angle * pi / 180),
						bulletStart.y = ppos.y + p1_offset.y * std::cos(angle * pi / 180) + p1_offset.x * std::sin(angle * pi / 180);
					}
					else
					{
						player.left = true;
						bulletStart.x = ppos.x + p2_offset.x * std::cos(angle * pi / 180) - p2_offset.y * std::sin(angle * pi / 180),
						bulletStart.y = ppos.y + p2_offset.y * std::cos(angle * pi / 180) + p2_offset.x * std::sin(angle * pi / 180);
					}

					Entity laser(redLaser, 0, weapon.damage);
					laser.sprite.setPosition(bulletStart);
					laser.sprite.setRotation(player.sprite.getRotation() - 180);
					lasers.push_back(laser);
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				if (weapon.automatic)
				{
					lmbDown = false;
				}
			}
		}

		if (lmbDown && weapon.automatic)
		{
			if(weapon.canShoot())
			{
				sf::Vector2f bulletStart(0, 0);
				if (player.left)
				{
					player.left = false;
					bulletStart.x = ppos.x + p1_offset.x * std::cos(angle * pi / 180) - p1_offset.y * std::sin(angle * pi / 180),
						bulletStart.y = ppos.y + p1_offset.y * std::cos(angle * pi / 180) + p1_offset.x * std::sin(angle * pi / 180);
				}
				else
				{
					player.left = true;
					bulletStart.x = ppos.x + p2_offset.x * std::cos(angle * pi / 180) - p2_offset.y * std::sin(angle * pi / 180),
						bulletStart.y = ppos.y + p2_offset.y * std::cos(angle * pi / 180) + p2_offset.x * std::sin(angle * pi / 180);
				}

				Entity laser(redLaser, 0, weapon.damage);
				laser.sprite.setPosition(bulletStart);
				laser.sprite.setRotation(player.sprite.getRotation() - 180);
				lasers.push_back(laser);
			}
		}

		crosshair.setPosition(mousePosition);

		player.sprite.setRotation(angle + 90);
		point1.setRotation(angle + 90);
		point1.setPosition(ppos);
		point2.setRotation(angle + 90);
		point2.setPosition(ppos);

		if (!(ppos.x - 4 < mousePosition.x && ppos.x + 4 > mousePosition.x && ppos.y - 4 < mousePosition.y && ppos.y + 4 > mousePosition.y))
		{
			sf::Vector2f movement(1.75f * delta * acceleration.y * cos(angle * pi / 180), 1.75f * delta * acceleration.y * sin(angle * pi / 180));
			player.sprite.move(movement);
			view.move(movement);
		}

		if ((!direction[0] && acceleration.y < 0) || deacceleration[0])
		{
			acceleration.y += 2.f;
			if (acceleration.y >= 0) deacceleration[0] = false;
		}
		else if ((!direction[1] && acceleration.y > 0) || deacceleration[1])
		{
			acceleration.y -= 2.f;
			if (acceleration.y <= 0) deacceleration[1] = false;
		}

		if (direction[0] && acceleration.y > -150)
			acceleration.y -= 2.f;
		else if (direction[1] && acceleration.y < 150)
			acceleration.y += 2.f;
		
		window.clear();

		for (int i = 0; i < lasers.size(); i++)
		{
			lasers[i].move(2000, delta);
			window.draw(lasers[i].sprite);
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			sf::Vector2f enemyPos = enemies[i].sprite.getPosition();

			enemies[i].sprite.setRotation(rotateTowards(enemyPos, ppos) + 90);

			float enemyAngle = (enemies[i].sprite.getRotation() - 90) * pi / 180;

			float distance = sqrt(pow(enemyPos.x - ppos.x, 2) + pow(enemyPos.y - ppos.y, 2));

			if (enemies[i].deacceleration[0])
			{
				enemies[i].sprite.move(1.f * delta * enemies[i].acceleration * -cos(enemies[i].originalAngle * pi / 180), 1.f * delta * enemies[i].acceleration * -sin(enemies[i].originalAngle * pi / 180));
				enemies[i].acceleration -= 2;
				if (enemies[i].acceleration <= 0)
				{
					enemies[i].acceleration = 150.f;
					enemies[i].deacceleration[0] = false;
				}
			}
			else if (enemies[i].deacceleration[1])
			{
				enemies[i].sprite.move(1.f * delta * enemies[i].acceleration * -cos(enemies[i].originalAngle * pi / 180), 1.f * delta * enemies[i].acceleration * -sin(enemies[i].originalAngle * pi / 180));
				enemies[i].acceleration += 2;
				if (enemies[i].acceleration >= 0)
				{
					enemies[i].acceleration = 150.f;
					enemies[i].deacceleration[1] = false;
				}
			}

			if (distance > 360)
			{
				enemies[i].sprite.move(delta * enemies[i].acceleration * -cos(enemyAngle), delta * 150.f * -sin(enemyAngle));
			}

			else if (Collision::BoundingBoxTest(enemies[i].sprite, player.sprite) && Collision::PixelPerfectTest(enemies[i].sprite, player.sprite))
			{
				sf::Vector2f newAcceleration = sf::Vector2f(-acceleration.x / 1.5, -acceleration.y / 1.5);
				if (acceleration.x >= 0)
				{
					if (acceleration.x < 20) acceleration.x = 20;
				}
				else
				{
					if (acceleration.x > -20) acceleration.x = -20;
				}

				if (acceleration.y >= 0)
				{
					if (acceleration.y < 20) acceleration.y = 20;
				}
				else
				{
					if (acceleration.y > -20) acceleration.y = -20;
				}

				for (int j = 0; j < 4; j++)
				{
					if (direction[j])
					{
						switch (j)
						{
						case 0:
							deacceleration[1] = true;
							enemies[i].deacceleration[1] = true;
							enemies[i].acceleration = acceleration.y/1.5;
							enemies[i].originalAngle = angle - 180;
							break;
						case 1:
							deacceleration[0] = true;
							enemies[i].deacceleration[0] = true;
							enemies[i].acceleration = acceleration.y/1.5;
							enemies[i].originalAngle = angle - 180;
							break;
						case 2:
							deacceleration[3] = true;
							break;
						case 3:
							deacceleration[2] = true;
							break;
						}
					}
				}

				acceleration = newAcceleration;
			}

			window.draw(enemies[i].sprite);
		}

		window.setView(view);

		window.draw(player.sprite);
		window.draw(crosshair);
		window.display();
	}
}