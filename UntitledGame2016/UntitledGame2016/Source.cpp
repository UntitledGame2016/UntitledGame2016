#include <SFML/Graphics.hpp>
#include "Hero.h"
#include "Weapons.h"
#include "Mobs.h"
#include "TextureManager.h"

using namespace Collision;

int main() {
	//Render Window
	float height = 1080;
	float width = 540;
	sf::ContextSettings settings;
	sf::RenderWindow window;
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(height, width, desktop.bitsPerPixel), "Untitled Game",
		sf::Style::Default, settings);
	window.setFramerateLimit(60);

	//Time
	sf::Clock cl;
	sf::Time elapsed;

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
	Hero hero({ 0, 250 });
	int weaponIndex = 0;

	std::vector<Weapon *> weapons;
	weapons.push_back(new Ranged("Gun", 10, { 0, 0, 64, 64 }));
	weapons.push_back(new Ranged("Assault Rifle", 50, { 0, 0, 64, 64 }, 100000.0f));
	weapons.push_back(new Ranged("Machine Gun", 200, { 0, 0, 64,64 }, 25000.0f));

	//Mobs
	std::vector<Mob *> mobs;
	mobs.push_back(new Mob(textures, { 400, 436 }, 100));

	//Level design
	std::vector<Block *> blocks;
	std::vector<std::pair<sf::Vector2u, float>> blockScript;
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 800, 300 }, 1.0f));
	
	blocks.push_back(new Block({ 1000, 50 }, { 0, 500 }, "box.png"));
	blocks.push_back(new Block({ 50, 50 }, { 300, 350 }, "box2.png"));
	blocks.push_back(new Block({ 1000, 50 }, { 540, 400 }, "box.png"));
	blocks.push_back(new Block({ 50, 50 }, { 0, 300 }, "box2.png", &blockScript));
	
	while (window.isOpen()) {

// -- Events -- 
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
			weapons[weaponIndex]->attack(hero.getPosition(), hero.face());

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) 
			weaponIndex = 0;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) 
			weaponIndex = 1;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
			weaponIndex = 2;
		/*
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			weapons[weaponIndex]->reload(5);
		*/
// -- Logic --
	//Gravity

	//Update
		hero.update(cl.getElapsedTime().asSeconds(), blocks);
		blocks[3]->update(cl.getElapsedTime().asSeconds());
		//std::cout << cl.getElapsedTime().asMicroseconds() << std::endl;

		for(Weapon * w: weapons)
			w->update(cl.getElapsedTime().asMicroseconds(), mobs);
		mobs[0]->update(cl.getElapsedTime().asMicroseconds());

		elapsed = cl.restart();


// -- Draw --
		window.clear();
		window.draw(background);

		//Environment
		for (Block * b : blocks)
			b->draw(window);

		//Player
		hero.draw(window);
		mobs[0]->draw(window);

		//Weapons
		for (Weapon * w : weapons)
			w->drawBullets(window);
		weapons[weaponIndex]->draw(window);

		window.display();
	}
	return 0;
}