#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <SFML\Graphics.hpp>
#include "Collision.h"
#include "TextureManager.h"

class Block {
	TextureManager textures;
	sf::RectangleShape block;
	sf::Texture blockTexture;
	sf::Sprite blockSprite;
public:
	Block(sf::Vector2f size, sf::Vector2f newPos, const std::string & fileName);
	sf::Sprite & getSprite();
	void move(sf::Vector2f distance);
	void draw(sf::RenderWindow & window);
	void showHitBox();
	bool colliding(sf::RectangleShape box);
	sf::Vector2f getSize();
	float getX();
	float getY();
	~Block();
};

#endif