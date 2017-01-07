#ifndef HERO_H
#define HERO_H

#include <iostream>
#include <SFML\Graphics.hpp>
#include "Foreign.h"
#include "Collision.h"
#include "TextureManager.h"

class Hero {
	sf::Text name;
	sf::Font nameFont;
	sf::RectangleShape heroInterface;
	sf::RectangleShape healthBar;
	TextureManager textures;
	sf::Texture heroTexture;
	sf::Sprite heroSprite;
	int health = 100;
public:
	sf::RectangleShape hitbox; //hitbox
	const float cooldown = 500000;
	float delay = 0;
	Hero(sf::Vector2f newPos, const std::string& fileName) {
		textures.addTexture(fileName);
		heroTexture = textures.loadTexture(fileName);
		heroSprite.setPosition(newPos);
		heroSprite.setTexture(heroTexture);
		heroSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));

		Collision::CreateTextureAndBitmask(heroTexture, fileName);

		hitbox.setSize({36, 63});
		hitbox.setPosition(newPos.x + 14, newPos.y + 1);
		hitbox.setFillColor(sf::Color::Transparent);

		heroInterface.setSize({ 304, 19 });
		heroInterface.setPosition({ 48, 48 });
		heroInterface.setFillColor(sf::Color::Black);
		if (!nameFont.loadFromFile("fonts/arial.ttf")) {
			std::cout << "Font not loaded" << std::endl;
		}
		name.setString("Hero Desu");
		name.setCharacterSize(18);
		name.setFont(nameFont);
		name.setFillColor(sf::Color::White);
		name.setOutlineColor(sf::Color::Black);
		name.setOutlineThickness(2);
		name.setStyle(sf::Text::Bold);
		name.setPosition({ 52, 28 });
		healthBar.setSize({ health * 3.0f, 15 });
		healthBar.setPosition({ 50, 50 });
		healthBar.setFillColor(sf::Color::Red);
	}
	
	void draw(sf::RenderWindow &window) {
		window.draw(heroInterface);
		window.draw(healthBar);
		window.draw(heroSprite);
		window.draw(name);
		window.draw(hitbox);
	}

	void move(sf::Vector2f distance) {
		if (health > 0) {
			heroSprite.move(distance);
			hitbox.move(distance);
		}
	}

	void update(float time) {
		//std::cout << delay << std::endl;
		delay -= time;
	}

	void changeHealth(const int x) {
		if (health > 0 && health + x <= 100)
			health += x;
		if(health <= 0)
			name.setString("You're dead LUL");
		healthBar.setSize({ health * 3.0f, 15 });
		std::cout << "Health: " << health << std::endl;
	}

	sf::Sprite getSprite() {
		return heroSprite;
	}

	//bounding test
	bool BBcollide(const sf::Sprite &obj2) {
		return Collision::BoundingBoxTest(heroSprite, obj2);
	}

	//For Foreign Objects (does not use perfect pixel collision)
	bool GBcollide(Foreign &object) {
		if (delay <= 0) {
			if (hitbox.getGlobalBounds().intersects(object.getGlobalBounds())) {
				delay = cooldown;
				return true;
			}
			return false;
		}
		return false;
	}

	sf::FloatRect getGlobalBounds() {
		return hitbox.getGlobalBounds();
	}
	sf::FloatRect getGlobalBounds2(){
		return heroSprite.getGlobalBounds();
	}

	void showHitBox() {
		hitbox.setFillColor(sf::Color::Blue);
	}

	void setTextureRect(sf::IntRect &newRect) {
		heroSprite.setTextureRect(newRect);
	}

	int getY() {
		return heroSprite.getPosition().y;
	}

	int getX() {
		return heroSprite.getPosition().x;
	}
	void setX(float position) {
		heroSprite.setPosition({ position,heroSprite.getPosition().y });
		hitbox.setPosition(heroSprite.getPosition().x + 14, position + 1);
	}
	void setY(float position) {
		heroSprite.setPosition({ heroSprite.getPosition().x, position });
		hitbox.setPosition(heroSprite.getPosition().x + 14, position + 1);
	}
};

#endif