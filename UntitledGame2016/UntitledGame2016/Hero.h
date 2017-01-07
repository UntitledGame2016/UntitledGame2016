#ifndef HERO_H
#define HERO_H

#include <iostream>
#include <SFML\Graphics.hpp>
#include "Foreign.h"
#include "Collision.h"
#include "TextureManager.h"

class Hero {
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
	}
	
	sf::Sprite getSprite() {
		return heroSprite;
	}

	//pixel perfect
	/*bool collisionTest(const sf::Sprite &obj2) {
		if (delay <= 0) {
			if (Collision::PixelPerfectTest(heroSprite, obj2)) {
				delay = cooldown;
				return true;
			}
			return false;
		}
		return false;
	}*/

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

	void takeDamage(const int x) {
		health -= x;
		std::cout << "Health: " << health << std::endl;
	}

	void setTextureRect(sf::IntRect &newRect) {
		heroSprite.setTextureRect(newRect);
	}

	void draw(sf::RenderWindow &window) {
		window.draw(heroSprite);
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