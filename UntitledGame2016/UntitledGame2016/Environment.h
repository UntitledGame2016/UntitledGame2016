#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Collision.h"

class Block {
	TextureManager textures;
	float rotAngle;
	sf::RectangleShape block;
	sf::Texture blockTexture;
	sf::Sprite blockSprite;
public:
	Block(sf::Vector2f size, sf::Vector2f newPos, const std::string& fileName, float angle = 0) {
		rotAngle = angle;
		textures.addTexture(fileName);
		blockTexture = textures.loadTexture(fileName);
		blockSprite.rotate(angle);
		blockTexture.setRepeated(true);
		blockSprite.setPosition(newPos);
		blockSprite.setTexture(blockTexture);
		//blockSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));

		Collision::CreateTextureAndBitmask(blockTexture, fileName);

		//block.rotate(angle);
		block.setSize(size);
		block.setPosition(newPos);
		block.setFillColor(sf::Color::Transparent);
	}

	sf::Sprite& getSprite() {
		return blockSprite;
	}

	void move(sf::Vector2f distance) {
		block.move(distance);
	}

	void draw(sf::RenderWindow &window) {
		window.draw(blockSprite);
		window.draw(block);
	}

	void showHitBox() {
		block.setFillColor(sf::Color::Green);
	}

	bool colliding(Hero *hero) {
		return block.getGlobalBounds().intersects(hero->hitbox.getGlobalBounds());
	}

	float getAngle() {
		return rotAngle;
	}
	int getX() {
		return block.getPosition().x;
	}

	sf::Vector2f getSize() {
		return block.getSize();
	}
	int getY() {
		return block.getPosition().y - 64; 
	}

	~Block() {
		std::cout << "Block is destroyed" << std::endl;
	}
};