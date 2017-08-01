#ifndef HERO_H
#define HERO_H

#include <math.h>
#include "Environment.h"
#include "Guardian.h"

class Hero {
private:
	sf::Texture heroTexture;
	sf::Sprite heroSprite;
	Animation * animation;
	TextureManager * allTextures;

	bool faceRight;
	unsigned int row;
public:
	sf::RectangleShape hitbox;
	Hero(sf::Vector2f newpos, TextureManager &textures);
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

	int gIndex;
	std::vector<Guardian *> guardians;
	float changeWindow = 0;
	bool alive = true;
	bool god = false;
public:
	void changeHealth(double x);
	void godMode(bool toggle);
	bool dead();
	void deathAnimation(float time);
private:
	std::vector<Weapon *> weapons;
	Weapon * weapon;
	int weapon_index = 0;
public:
	Weapon* getWeapon();
	void wield(Weapon * w);
private:
	//Gravity
	bool jumping = true;
	bool fallRight = false;
	bool fallLeft = false;
	bool jumpRight = false;
	bool jumpLeft = false;
	int curr = 0;			//Block index
	float jumpSpeed = 0;	//Jump is 15 pixels
	float moveSpeed = 5.0f; //DONT FUCKING CHANGE THIS
	float maxSpeed = 10.0f;
	const float gravity = 0.75;	

	sf::Vector2f view;
public:
	float getY();
	float getX();
	void setX(float position);
	void setY(float position);
	bool face();
	float getSpeed();
	void changeMaxSpeed(float newMaxSpeed);

	void update(float time, sf::Vector2f viewCoor, std::vector<Block *> blocks, std::vector<Mob *> &mobs);
	void draw(sf::RenderWindow& window);
	void drawHUD(sf::RenderWindow& window);
	void move(sf::Vector2f distance);
	void stop(sf::Vector2f distance);

	bool BBcollide(const sf::Sprite & obj2);
	sf::FloatRect getGlobalBounds();
	sf::FloatRect getGlobalBounds2();
};



#endif