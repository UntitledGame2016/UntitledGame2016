#ifndef MOBS_H
#define MOBS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Collision.h"
#include "FileManager.h"

class Mob {
	sf::Sprite sprite;
	sf::RectangleShape hitbox;
	sf::RectangleShape healthbar;
	sf::Texture texture;
	AudioManager mobSounds;
	sf::Sound mobDeath;

	float barLength;
	float health;
	float maxHealth;
	float damage;
public:
	Mob(TextureManager &textures, sf::Vector2f newpos, int health, float newDamage = 5);
	void draw(sf::RenderWindow &window);
	void setPosition(sf::Vector2f pos);
	bool collide(sf::Sprite &obj);
	bool collide(sf::RectangleShape &obj);
	sf::Sprite getSprite() { return sprite; }
	void changeHealth(const int hp);
	float getDamage();
	void update(float time);
	bool dead();
};

#endif