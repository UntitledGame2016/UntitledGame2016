#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <math.h>
#include "Hero.h"
#include "Environment.h"
#include "Foreign.h"
#include "TextureManager.h"

using namespace Collision;

int main()
{
	//Render Window
	float height = 1080;
	float width = 540;
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window;
	window.create(sf::VideoMode(height, width, desktop.bitsPerPixel), "Untitled Game");
	window.setFramerateLimit(60);

	//Logic variables
	sf::Clock cl;
	sf::Clock clock;
	sf::Time elapsed;
	bool fall = true;
	bool jumping = false;
	bool fallRight = false;
	bool fallLeft = false;
	int animation = 0;
	int delayCounter = 0;
	int curr = 0;			//Block index
	float jumpSpeed = -15;
	float fallSpeed = 0;
	const float gravity = 1.0;
	const float moveSpeed = 3.5; //DONT FUCKING CHANGE THIS

	//Objects (size, position) 
	sf::RectangleShape background;
	background.setFillColor(sf::Color::Yellow);
	background.setSize({ height, width });

	Hero p({ 0, 250 }, "sample_spritesheet.png");

	std::vector<Block> blocks;
	Block b({ 1000, 50 }, { 0, 500 }, "box.png");
	Block c({ 50, 50 }, { 300, 350 }, "box2.png", 30);
	Block platform({ 100, 100 }, { 540, 400 }, "box.png");
	blocks.push_back(b);
	blocks.push_back(c);
	blocks.push_back(platform);
	b.showHitBox();
	c.showHitBox();
	platform.showHitBox();
	p.showHitBox();

	std::vector<Foreign *> missile;
	std::vector<Foreign *> potions;
	std::vector<Foreign *> usedPotions;
	missile.push_back(new Foreign({ 500, 460 }, "dud.png"));
	potions.push_back(new Foreign({ 300 , 430 }, "potion.png"));
	potions.push_back(new Foreign({ 350 , 430 }, "potion.png"));
	potions.push_back(new Foreign({ 400 , 430 }, "potion.png"));

	while (window.isOpen()) {
		bool collisions = false;	//collision?

// -- Events -- 
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Keys
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !jumping) {
			p.move({ 0, jumpSpeed });
			jumping = true;
			for (int i = 0; i < blocks.size(); i++) {
				if (curr != i && blocks[i].colliding(&p)) {
					std::cout << "Bottom Collision" << std::endl;
					jumping = false;
					jumpSpeed = 0;
					break;
				}
			}
			curr = -1;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			p.setTextureRect(sf::IntRect(animation * 64, 0, 64, 64));
			p.move({ moveSpeed, 0 });
			for (int i = 0; i < blocks.size(); i++) {
				if (curr != i && blocks[i].colliding(&p)) {
					std::cout << "Left Collision" << std::endl;
					p.move({ -moveSpeed, 0 });
					break;
				}
			}
			if (fallLeft) {
				p.move({ -moveSpeed, 0 });
			}
			if (!jumping && !blocks[curr].colliding(&p)) {
				fallRight = true;
				jumpSpeed = 0;
				jumping = true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			p.setTextureRect(sf::IntRect(animation * 64, 64, 64, 64));
			p.move({ -moveSpeed, 0 });
			for (int i = 0; i < blocks.size(); i++) {
				if (curr != i && blocks[i].colliding(&p)) {
					std::cout << "Right Collision" << std::endl;
					p.move({ moveSpeed, 0 });
					break;
				}
			}
			if (fallRight) {
				p.move({ moveSpeed, 0 });
			}
			if (!jumping && !blocks[curr].colliding(&p)) {
				fallLeft = true;
				jumpSpeed = 0;
				jumping = true;
			}
		}

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

// -- Logic --
	//Animation
		delayCounter++;
		if (delayCounter % 5 == 0) {
			animation++;
		}
		if (animation == 2) {
			animation = 0;
		}

	//Gravity
		if (cl.getElapsedTime().asMicroseconds() > 800.0f) {
			if (fall && !jumping && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { //If falling
				if (p.getY() < blocks[curr].getY()) {
					fallSpeed += gravity;
					p.move({ 0, fallSpeed });
					for (int i = 0; i < blocks.size(); i++) {
						if (blocks[i].colliding(&p))
							p.setY(blocks[i].getY());
					}
				}
				else {
					fallSpeed = 0;
					fall = false;
				}
			}
			else if (jumping) {
				for (int i = 0; i < blocks.size(); i++) { //If landing
					int tempSpeed = jumpSpeed;
					p.move({ 0, jumpSpeed + gravity });
					if (blocks[i].colliding(&p)) {
						// if the hero is colliding but the bottom of the hero is still below the platform
						if (p.getY() > blocks[i].getY() + blocks[i].getSize().y) {
							// sets the top of the hero to bottom of platform
							p.setY(blocks[i].getY() + blocks[i].getSize().y + 64);
							collisions = true;
							jumpSpeed = 0;
							break;
						}
						else {
							p.setY(blocks[i].getY());
							jumpSpeed = -15;
							jumping = false, fallLeft = false, fallRight = false;
							curr = i;
							collisions = true;
							break;
						}
					}
					p.move({ 0, -tempSpeed - gravity });
				}
				if (!collisions) { //If still in the air
					jumpSpeed += gravity;
					p.move({ 0, jumpSpeed });
				}
			}
		}

	//Foreign Collisions
		//Pixel perfect for pixel perfect images
		/*if (p.collisionTest(b.getSprite())) {
			std::cout << "Pixel Perfect!" << delayCounter << std::endl;
		}

		if (p.collision(b.getSprite())) {
			std::cout << "Bounding box!" << delayCounter << " " << curr << " " << collisions << std::endl;
		}*/

		for (size_t index = 0; index < missile.size(); index++) {
			missile[index]->fire();
			if (p.GBcollide(*missile[index])) {
				p.changeHealth(-50);
			}
		}

		for (size_t index = 0; index < potions.size(); index++) {
			if (p.GBcollide(*potions[index])) {
				p.changeHealth(50);
				if (potions.size() < 1)
					potions.push_back(new Foreign({ 50, 50 }, "potion.png"));
			}
		}
		

	//Clock
		p.update(cl.getElapsedTime().asMicroseconds());
		//std::cout << cl.getElapsedTime().asMicroseconds() << std::endl;
		cl.restart();

// -- Draw --
		window.clear();
		window.draw(background);
		platform.draw(window);
		//platform1.draw(window);
		//platform2.draw(window);
		for (size_t index = 0; index < missile.size(); index++) {
			missile[index]->draw(window);
		}
		for (size_t index = 0; index < potions.size(); index++) {
			potions[index]->draw(window);
		}
		b.draw(window);
		c.draw(window);
		p.draw(window);

		window.display();
	}

	return 0;
}