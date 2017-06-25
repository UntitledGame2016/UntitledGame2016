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
	int maxhp = 100;
	int hp = maxhp;
	int hpindex = 0;

	std::vector<Weapon *> weapons;
	Weapon * weapon;

public:
	void changeHealth(const int x);
	void wield(Weapon * w);
	Weapon& getWeapon();

private:
	//Gravity
	bool fall = true;
	bool jumping = false;
	bool fallRight = false;
	bool fallLeft = false;
	int curr = 0;			//Block index
	float jumpSpeed = -15;
	const float moveSpeed = 3.5; //DONT FUCKING CHANGE THIS
	float fallSpeed = 0;
	const float gravity = 1.0;

public:
	float getY();
	float getX();
	void setX(float position);
	void setY(float position);
	bool face();

	void update(float time, std::vector<Block *> blocks, std::vector<Mob *> &mobs);
	void draw(sf::RenderWindow & window);
	void stop(sf::Vector2f distance);
	void move(sf::Vector2f distance);

	bool BBcollide(const sf::Sprite & obj2);
	sf::FloatRect getGlobalBounds();
	sf::FloatRect getGlobalBounds2();


};



#endif