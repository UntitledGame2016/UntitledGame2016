#ifndef GUARDIAN_H
#define GUARDIAN_H

#include <iostream>
#include "FileManager.h"
#include "Weapons.h"
//#include "Animation.h"

class Hero;

class Guardian {
	Animation * animation;
protected:
	Hero * heroptr;
	int level;
	int loyalty;
	bool active;
	float elapsed = 0;
	sf::Text nameText;
	sf::Font nameFont;
	std::string name;
	sf::CircleShape icon;
	TextureManager * guardianTextures;
	sf::Texture guardianTexture;
	sf::Sprite guardianSprite;
public:
	Guardian(Hero &hero, TextureManager &textures);
	std::string getName();
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
	Estelle(Hero &hero, TextureManager &textures);
	void heroActive();
	void update(float time);
};

class Evangeline : public Guardian {

public:
	Evangeline(Hero &hero, TextureManager &textures);
	void heroActive();
	void update(float time);
};

class Aiden : public Guardian {

public:
	Aiden(Hero &hero, TextureManager &textures);
	void heroActive();
	void update(float time);
};

#endif 