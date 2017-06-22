#include "Environment.h"

Block::Block(sf::Vector2f size, sf::Vector2f newPos, const std::string& fileName, std::vector<std::pair<sf::Vector2u, float>> * scr) : moving(moving) {
	textures.addTexture(fileName);
	blockTexture = textures.loadTexture(fileName);
	blockTexture.setRepeated(true);
	blockSprite.setPosition(newPos);
	blockSprite.setTexture(blockTexture);
	//blockSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));

	script = scr;
	moving = true;

	block.setSize(size);
	block.setPosition(newPos);
	block.setFillColor(sf::Color::Red);

}

Block::Block(sf::Vector2f size, sf::Vector2f newPos, const std::string& fileName) {
	textures.addTexture(fileName);
	blockTexture = textures.loadTexture(fileName);
	blockTexture.setRepeated(true);
	blockSprite.setPosition(newPos);
	blockSprite.setTexture(blockTexture);
	//blockSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));

	block.setSize(size);
	block.setPosition(newPos);
	block.setFillColor(sf::Color::Transparent);
	
}

void Block::update(float deltaTime) {
	int index = 0;
	float x = (*script)[index].second;
	float y = (*script)[index].second;

	if(index < script->size()){
		if ((*script)[index].first.x > block.getPosition().x)
			x = abs(x);
		else if ((*script)[index].first.x < block.getPosition().x)
			x = -abs(x);
		if ((*script)[index].first.y > block.getPosition().y)
			y = abs(y);
		else if ((*script)[index].first.y < block.getPosition().y)
			y = -abs(y);

		move({ x, y });
		//std::cout << (*script)[index].first.x << " " << (*script)[index].first.y << std::endl;
		
		if((*script)[index].first.x == block.getPosition().x && (*script)[index].first.y == block.getPosition().y)
			index++;
	}

	//animation
}

sf::Sprite& Block::getSprite() {
	return blockSprite;
}

void Block::move(sf::Vector2f distance) {
	block.move(distance);
	blockSprite.move(distance);
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