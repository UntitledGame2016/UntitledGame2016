#ifndef GUARDIAN_H
#define GUARDIAN_H

#include <iostream>
#include "TextureManager.h"
#include "Weapons.h"
//#include "Animation.h"

class Hero;

class Guardian {
	sf::Sprite guardianSprite;
	Animation * animation;
	TextureManager textures;
protected:
	Hero * heroptr;
	int level;
	int loyalty;
	bool active;
	float elapsed = 0;
	sf::CircleShape icon;
public:
	Guardian(Hero &hero);
	virtual void heroActive() {};
	virtual void update(float time);
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();
	void move(sf::Vector2f distance);
	void levelUp(int inc);
	void draw(sf::RenderWindow &window);
};

class Estelle : public Guardian {

public:
	Estelle(Hero &hero);
	void heroActive();
	void update(float time);
};

class Evangeline : public Guardian {

public:
	Evangeline(Hero &hero);
	void heroActive();
	void update(float time);
};

class Aiden : public Guardian {

public:
	Aiden(Hero &hero);
	void heroActive();
	void update(float time);
};

#endif 