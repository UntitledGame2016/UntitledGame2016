#ifndef MOBS_H
#define MOBS_H

#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "TextureManager.h"

class Mob {
	sf::Sprite sprite;
	sf::RectangleShape hitbox;
	sf::RectangleShape healthbar;
	sf::Texture texture;

	float barLength;
	float health;
	float maxHealth;
public:
	Mob(TextureManager &textures, sf::Vector2f newpos, int health);
	void draw(sf::RenderWindow &window);
	void setPosition(sf::Vector2f pos);
	bool collide(sf::Sprite &obj);
	bool collide(sf::RectangleShape &obj);
	sf::Sprite getSprite() { return sprite; }
	void changeHealth(const int hp);
	void update(float time);
	bool dead();
};

#endif