#ifndef HERO_H
#define HERO_H

#include <math.h>
#include "Animation.h"
#include "Environment.h"
#include "Weapons.h"

class Hero {
private:
	sf::Texture heroTexture;
	sf::Sprite heroSprite;
	Animation * animation;
	TextureManager textures;

	bool faceRight;
	unsigned int row;
public:
	sf::RectangleShape hitbox;
	Hero(sf::Vector2f newpos);
	void showHitBox();
	sf::Sprite getSprite();
	sf::Vector2f getPosition();

private:
	sf::Font nameFont;
	sf::Text name;
	sf::Text deathMessage;
	sf::VertexArray healthCircle;
	sf::CircleShape healthOverlay;

	double pi = 3.1415926535897932384;
	double maxhp = 100;
	double hp = maxhp;
	int hpindex = 0;

	bool hero_active = true;
	bool alive = true;
public:
	void changeHealth(const double x);
	bool dead();
	void deathAnimation(float time);
private:
	std::vector<Weapon *> weapons;
	Weapon * weapon;
	int weapon_index = 0;
public:
	Weapon& getWeapon();
	void wield(Weapon * w);

private:
	//Gravity
	bool jumping = true;
	bool fallRight = false;
	bool fallLeft = false;
	int curr = 0;			//Block index
	float jumpSpeed = 0;
	float moveSpeed = 3.5f; //DONT FUCKING CHANGE THIS
	const float maxSpeed = 20.0f;
	const float gravity = 1.0;

public:
	float getY();
	float getX();
	void setX(float position);
	void setY(float position);
	bool face();

	void update(float time, std::vector<Block *> blocks, std::vector<Mob *> &mobs);
	void draw(sf::RenderWindow & window);
	void move(sf::Vector2f distance);
	void stop(sf::Vector2f distance);

	bool BBcollide(const sf::Sprite & obj2);
	sf::FloatRect getGlobalBounds();
	sf::FloatRect getGlobalBounds2();


};



#endif