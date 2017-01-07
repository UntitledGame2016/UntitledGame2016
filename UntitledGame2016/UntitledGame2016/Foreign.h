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
		hitbox.setFillColor(sf::Color::Green);

		textures.addTexture(fileName);
		objectTexture = textures.loadTexture(fileName);
		objectSprite.setPosition({ newPos.x - (objectTexture.getSize().x - hitbox.getSize().x)/2,
			newPos.y - (objectTexture.getSize().y - hitbox.getSize().y) / 2 });
		objectSprite.setTexture(objectTexture);
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

	void fire() {
		hitbox.move({ -1.0f, 0 });
		objectSprite.move({ -1.0f, 0 });
	}

	void draw(sf::RenderTarget &window) {
		window.draw(objectSprite);
		//window.draw(hitbox);
	}
};

#endif 