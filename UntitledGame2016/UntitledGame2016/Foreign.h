#ifndef FOREIGN_H
#define FOREIGN_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Foreign {
	TextureManager textures;
	sf::RectangleShape hitbox; //hitbox 
	sf::Texture objectTexture;
	sf::Sprite objectSprite;
public:
	Foreign(sf::Vector2f newPos, const std::string &fileName) {
		hitbox.setSize({ 15, 15});
		hitbox.setPosition(newPos);
		hitbox.setFillColor(sf::Color::Transparent);

		textures.addTexture(fileName);
		objectTexture = textures.loadTexture(fileName);
		objectSprite.setPosition({ newPos.x - (objectTexture.getSize().x - hitbox.getSize().x)/2,
			newPos.y - (objectTexture.getSize().y - hitbox.getSize().y) / 2 });
		objectSprite.setTexture(objectTexture);
	}

	void showHitBox() {
		hitbox.setFillColor(sf::Color::Green);
	}

	void setPosition(sf::Vector2f position) {
		hitbox.setPosition({ position.x, position.y });
		objectSprite.setPosition({ position.x, position.y });
	}

	sf::FloatRect getGlobalBounds() {
		return hitbox.getGlobalBounds();
	}

	sf::Sprite getSprite() {
		return objectSprite;
	}

	void disappear() { //Temporary solution to object manipulation
		hitbox.setPosition({ 5000, 5000 });
		objectSprite.setPosition({ 5000, 5000 });
	}

	void fire(const float velocity) {
		hitbox.move({ velocity, 0 });
		objectSprite.move({ velocity, 0 });
	}

	void draw(sf::RenderTarget &window) {
		window.draw(objectSprite);
		window.draw(hitbox);
	}
};

#endif 