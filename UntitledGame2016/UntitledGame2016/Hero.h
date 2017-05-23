#ifndef HERO_H
#define HERO_H

#include <iostream>
#include <SFML\Graphics.hpp>
#include <math.h>
#include <vector>
#include "Foreign.h"
#include "Collision.h"
#include "TextureManager.h"

double pi = 3.1415926535897932384;

class Hero {
	sf::VertexArray healthCircle;
	sf::CircleShape healthOverlay;
	sf::Text name;
	sf::Text deathMessage;
	sf::Font nameFont;
	TextureManager textures;
	sf::Texture heroTexture;
	sf::Sprite heroSprite;
	int maxhp = 100;
	int hp = maxhp;
	int hpindex = 0;
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
		name.setPosition({ 50, 25 });

		deathMessage.setFont(nameFont);
		deathMessage.setFillColor(sf::Color::White);
		deathMessage.setStyle(sf::Text::Bold);
		deathMessage.setPosition({ 75.0f, 80.0f });

		healthOverlay.setRadius(50.0f);
		healthOverlay.setPosition({ 50.0f, 50.0f });
		healthOverlay.setFillColor(sf::Color::Transparent);
		healthOverlay.setOutlineColor(sf::Color::Black);
		healthOverlay.setOutlineThickness(2);

		healthCircle.resize(hp);
		healthCircle.setPrimitiveType(sf::TrianglesFan);
		double angle = - pi / 2;
		for (size_t i = 0; i < maxhp; i++) {
			//x = center.x + radius*cos(t) , y = center.y + radius*sin(t)
			float x = 100.0f + 50.0f * std::cos(angle);
			float y = 100.0f + 50.0f * std::sin(angle);
			healthCircle[i].position = sf::Vector2f(x, y);
			healthCircle[i].color = sf::Color::Red;
			angle += pi / (maxhp / 2);
		}
	}
	
	void draw(sf::RenderWindow &window) {
		window.draw(heroSprite);
		window.draw(name);
		window.draw(hitbox);
		window.draw(healthCircle);
		window.draw(healthOverlay);
		window.draw(deathMessage);
	}

	void move(sf::Vector2f distance) {
		if (hp > 0) {
			heroSprite.move(distance);
			hitbox.move(distance);
		}
	}

	void update(float time) {
		//std::cout << delay << std::endl;
		delay -= time;
	}

	void changeHealth(const int x) {
		if (x < -maxhp) {
			std::cout << "Damage cannot exceed available health" << std::endl;
			return;
		}
		hp += x;
		if (hp > maxhp)
			hp = maxhp;
		double angle = 0;
		if (x < 0) { //When taking damage
			int temp = hpindex + abs(x);
			if (temp > maxhp)
				temp = maxhp;
			for (size_t i = hpindex; i < temp; i++) 
				healthCircle[i].position = sf::Vector2f(100.0f, 100.0f);
			hpindex = temp;
		}
		else if (x > 0) {
			int temp = hpindex - x;
			if (temp < 0)
				temp = 0;
			double angle = -pi / 2;
			for (size_t i = 0; i < maxhp; i++) {
				if (i >= temp && i < hpindex) {
					float x = 100.0f + 50.0f * std::cos(angle);
					float y = 100.0f + 50.0f * std::sin(angle);
					healthCircle[i].position = sf::Vector2f(x, y);
				}
				angle += pi / (maxhp / 2);
			}
			hpindex = temp;
		}
		if (hp <= 0) {
			name.setString("You're dead LUL");
			deathMessage.setString("RIP");
			healthOverlay.setFillColor(sf::Color::Black);
		}
		std::cout << "Health: " << hp << std::endl;
	}

	sf::Sprite getSprite() {
		return heroSprite;
	}

	//pixel perfect test
	bool PPcollide(Foreign &object) {
		return Collision::PixelPerfectTest(heroSprite, object.getSprite());
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

	sf::Vector2f getPosition() {
		return heroSprite.getPosition();
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