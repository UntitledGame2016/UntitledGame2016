#include <SFML/Graphics.hpp>
#include "Hero.h"
#include "Weapons.h"
#include "Mobs.h"
#include "TextureManager.h"

using namespace Collision;

int main() {
	//Render Window
	float height = 1920;
	float width = 1080;
	sf::ContextSettings settings;
	sf::RenderWindow window;
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(height, width, desktop.bitsPerPixel), "Untitled Game",
		sf::Style::Default, settings);
	window.setFramerateLimit(60);

	//Time
	sf::Clock cl;

	//Objects (size, position) 
	sf::RectangleShape background;
	background.setFillColor(sf::Color::Yellow);
	background.setSize({ height, width });

	//Textures
	TextureManager textures;
	textures.addTexture("weapons_spritesheet.png");
	textures.addTexture("sample_spritesheet.png");
	textures.addTexture("box.png");
	textures.addTexture("box2.png");
	textures.addTexture("mob.png");
	textures.addTexture("bullet.png");

	//Hero and Weapons 
	Hero hero({ 0, 786 });
	//hero.showHitBox();
	int weaponIndex = 0;

	//Mobs
	std::vector<Mob *> mobs;
	mobs.push_back(new Mob(textures, { 400, 914 }, 100));
	mobs.push_back(new Mob(textures, { 600, 876 }, 100));
	mobs.push_back(new Mob(textures, { 800, 876 }, 100));
	mobs.push_back(new Mob(textures, { 300, 814 }, 100));

	//Level design
	std::vector<Block *> blocks;
	std::vector<std::pair<sf::Vector2u, float>> blockScript;
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 100, 300 }, 1.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 0, 300 }, 1.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 300, 300 }, 1.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 300, 0 }, 1.0f));
	
	blocks.push_back(new Block({ 1000, 50 }, { 0, 1000 }, "box.png"));
	blocks.push_back(new Block({ 100, 100 }, { 0, 850 }));
	blocks.push_back(new Block({ 1000, 50 }, { 540, 900 }, "box.png"));
	blocks.push_back(new Block({ 50, 50 }, { 0, 300 }, "box2.png", &blockScript));
	
	while (window.isOpen()) {

// -- Events -- 
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

// -- Logic --
	//Gravity

	//Update
		if (!hero.dead())
			hero.update(cl.getElapsedTime().asSeconds(), blocks, mobs);
		else
			hero.deathAnimation(cl.getElapsedTime().asSeconds());
		//blocks[3]->update(cl.getElapsedTime().asSeconds());
		//std::cout << cl.getElapsedTime().asMicroseconds() << std::endl;
		cl.restart();

// -- Draw --
		window.clear();
		window.draw(background);

		//Environment
		for (Block * b : blocks)
			b->draw(window);

		//Player
		hero.draw(window);
		for (Mob * a : mobs) 
			a->draw(window);

		window.display();
	}
	return 0;
}