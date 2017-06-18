#include "Environment.h"

Block::Block(sf::Vector2f size, sf::Vector2f newPos, const std::string& fileName) {
	textures.addTexture(fileName);
	blockTexture = textures.loadTexture(fileName);
	blockTexture.setRepeated(true);
	blockSprite.setPosition(newPos);
	blockSprite.setTexture(blockTexture);
	//blockSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));

	//block.rotate(angle);
	block.setSize(size);
	block.setPosition(newPos);
	block.setFillColor(sf::Color::Transparent);
}

sf::Sprite& Block::getSprite() {
	return blockSprite;
}

void Block::move(sf::Vector2f distance) {
	block.move(distance);
}

void Block::draw(sf::RenderWindow &window) {
	window.draw(blockSprite);
	window.draw(block);
}

void Block::showHitBox() {
	block.setFillColor(sf::Color::Green);
}

bool Block::colliding(sf::RectangleShape box) {
	return block.getGlobalBounds().intersects(box.getGlobalBounds());
}


sf::Vector2f Block::getSize() {
	return block.getSize();
}

float Block::getY() {
	return block.getPosition().y - 64;
}

float Block::getX() {
	return block.getPosition().x;
}
Block::~Block() {
	std::cout << "Block is destroyed" << std::endl;
}