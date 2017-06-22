#include "Environment.h"

Block::Block(sf::Vector2f size, sf::Vector2f newPos, const std::string& fileName, std::vector<std::pair<sf::Vector2u, float>> * scr) : moving(moving) {
	textures.addTexture(fileName);
	blockTexture = textures.loadTexture(fileName);
	blockTexture.setRepeated(true);
	blockSprite.setPosition(newPos);
	blockSprite.setTexture(blockTexture);
	//blockSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));

	script = scr;
	scriptIndex = 0;
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
	if(scriptIndex < script->size()){
		float x = (*script)[scriptIndex].second;
		float y = (*script)[scriptIndex].second;

		if ((*script)[scriptIndex].first.x > block.getPosition().x)
			x = fabs(x);
		else if ((*script)[scriptIndex].first.x < block.getPosition().x)
			x = -fabs(x);
		else
			x = 0;

		if ((*script)[scriptIndex].first.y > block.getPosition().y)
			y = fabs(y);
		else if ((*script)[scriptIndex].first.y < block.getPosition().y)
			y = -fabs(y);
		else
			y = 0;

		move({ x, y });
		/*std::cout << x << " " << y << std::endl;
		std::cout << (*script)[scriptIndex].first.x << " " << (*script)[scriptIndex].first.y << std::endl;
		std::cout << block.getPosition().x << " " << block.getPosition().y << std::endl << std::endl;*/

		if ((*script)[scriptIndex].first.x == block.getPosition().x && (*script)[scriptIndex].first.y == block.getPosition().y)
			scriptIndex++;
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