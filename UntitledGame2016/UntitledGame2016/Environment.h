#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <SFML\Graphics.hpp>
#include "Collision.h"
#include "TextureManager.h"
#include <map>

class Block {
	TextureManager textures;
	sf::RectangleShape block;
	sf::Texture blockTexture;
	sf::Sprite blockSprite;

	std::vector<std::pair<sf::Vector2u, float>> * script; //coordinate vs speed
	int scriptIndex;
public:
	bool moving;
	Block(sf::Vector2f size, sf::Vector2f newPos, const std::string & fileName, std::vector<std::pair<sf::Vector2u, float>> * scr);
	Block(sf::Vector2f size, sf::Vector2f newPos, const std::string& fileName);
	Block(sf::Vector2f size, sf::Vector2f newPos);
	sf::Sprite & getSprite();
	void move(sf::Vector2f distance);
	void draw(sf::RenderWindow & window);
	void showHitBox();
	sf::Vector2f update(float deltaTime);
	bool colliding(sf::RectangleShape box);
	sf::Vector2f getSize();
	float getX();
	float getY();
	~Block();
};

#endif