#ifndef WEAPON_H
#define WEAPON_H

#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "Mobs.h"
#include "Animation.h"

/*								Bullet								*/
class Bullet {
	sf::RectangleShape hitbox;
	sf::Sprite bulletSprite;
	sf::Texture bulletTexture;
	bool fired = false;
	float velocity = 25.0f;
	sf::Vector2f maxRange;
public:
	Bullet();
	Bullet(TextureManager & textures, sf::IntRect bulletTexture);
	void draw(sf::RenderWindow & window);
	void move(sf::Vector2f velocity);
	void setPosition(sf::Vector2f pos, float range = 1000);
	void toggle(bool active);	
	bool isready();
	float getvel();
	bool collide(sf::Sprite & obj);
	void changeFaceRight(bool i);
	sf::Vector2f getHBSize();
	sf::Vector2f getPosition();
};

/*								Weapons								*/
class Weapon {
	sf::Text name;
protected:
	AudioManager weaponSounds;
	sf::Sound attackSound;
	sf::Sound reloadSound;
	sf::Sound wieldSound;
	sf::Sound swingSound;
	int randomSound = 0;
public:
	void playWield();
protected:
	float damage;
	bool type;
	bool attacking;
	unsigned int durability;
	unsigned int maxDurability;
	sf::Font font;
public:
	Weapon();
	Weapon(std::string name, int durability, float newDamage, sf::IntRect texture = { 0, 0, 64, 64 });
	virtual void attack(sf::Vector2f pos, bool faceRight) {};
	virtual void update(float time, std::vector<Mob *> &mobs, sf::Vector2f pos) {};
	virtual void draw(sf::RenderWindow &window);
	virtual void drawHUD(sf::RenderWindow &window);
	virtual void reload(int newBullets) {};
	virtual void repair(int newDura) {};
	virtual void showHitBox() {};
	virtual bool isattacking() { return false; };
	void changeDamage(float newDmg);
	bool isRanged();
	float getDamage();
};

class Ranged : public Weapon{
	std::vector<Bullet *> bullets;
	unsigned int bulletsfired = 0;
	unsigned int bulletindex = 0;
	float firerate;
	float delay = 0;
	float bulletRange;
	sf::Text weaponDura;
public:
	Ranged(std::string name, int durability, sf::IntRect textureRect, float newDamage = 5.0f, const float firerate = 0.45f, const float range = 1000.0f);
	void draw(sf::RenderWindow &window);
	void drawHUD(sf::RenderWindow &window);
	void attack(sf::Vector2f pos, bool faceRight);
	void update(float time, std::vector<Mob *> &mobs, sf::Vector2f pos);
	void reload(int newBullets);
};

class Melee : public Weapon {
	sf::RectangleShape hitbox;
	bool faceRight;
	float range;
	float attackSpeed;
	float delay = 0;
	sf::Text weaponDura;
public:
	Melee();
	Melee(std::string name, int durability, sf::IntRect textureRect, float newDamage = 10.0f, float newSpeed = 0.5f, float range = 50.0f);
	void update(float time, std::vector<Mob *> &mobs, sf::Vector2f pos);
	void attack(sf::Vector2f pos, bool faceRight);
	void draw(sf::RenderWindow &window);
	void repair(int newDura);
	void showHitBox();
	void drawHUD(sf::RenderWindow &window);
	bool isattacking();
};
#endif 