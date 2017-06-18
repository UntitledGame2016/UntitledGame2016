#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Collision.h"
#include "Mobs.h"

/*								Bullet								*/
class Bullet {
	sf::RectangleShape hitbox;
	sf::Sprite bulletSprite;
	sf::Texture bulletTexture;
	bool fired = false;
	float velocity = 10.0f;

public:
	Bullet();
	Bullet(TextureManager & textures, sf::IntRect bulletTexture);
	void draw(sf::RenderWindow & window);
	void move(sf::Vector2f velocity);
	void setPosition(sf::Vector2f pos);
	void toggle(bool active);
	bool isready();
	float getvel();
	bool collide(sf::Sprite & obj);
	void changeDir(bool i);
	sf::Vector2f getHBSize();
	sf::Vector2f getPosition();

};

/*								Weapons								*/
class Weapon {
	TextureManager textures;
	sf::Sprite weaponSprite;
	sf::Texture weaponTexture;

	sf::Text name;
protected:
	unsigned int durability;
	unsigned int maxDurability;
	sf::Font font;
public:
	Weapon();
	Weapon(std::string name, int durability, sf::IntRect texture = { 0, 0, 64, 64 });
	virtual void attack(sf::Vector2f pos, bool faceRight) {};
	virtual void update(float time, std::vector<Mob *> &mobs) {};
	virtual void draw(sf::RenderWindow &window);
	virtual void drawBullets(sf::RenderWindow &window) {};
	virtual void reload(int newBullets) {};
};


class Ranged : public Weapon{
	std::vector<Bullet *> bullets;
	unsigned int bulletsfired = 0;
	unsigned int bulletindex = 0;
	float firerate;
	float delay = 0;

	sf::Text clipText;
public:
	Ranged(std::string name, int durability, sf::IntRect textureRect, const float firerate = 300000);
	void draw(sf::RenderWindow &window);
	void drawBullets(sf::RenderWindow &window);
	void attack(sf::Vector2f pos, bool faceRight);
	void update(float time, std::vector<Mob *> &mobs);
	void reload(int newBullets);
};


#endif 