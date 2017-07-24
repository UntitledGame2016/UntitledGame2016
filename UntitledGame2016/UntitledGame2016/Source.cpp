#include <SFML/Graphics.hpp>
#include "Hero.h"
#include "Weapons.h"
#include "Mobs.h"
#include "TextureManager.h"

using namespace Collision;

int main() {
	//Render Window
	float height = 1080;
	float width = 1920;
	sf::ContextSettings settings;
	sf::RenderWindow window;
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(width, height, desktop.bitsPerPixel), "Untitled Game",
		sf::Style::Fullscreen, settings);
	window.setFramerateLimit(60);

	//Time
	sf::Clock cl;

	//Objects (size, position) 
	sf::RectangleShape background;
	background.setFillColor(sf::Color::Yellow);
	background.setSize({ width*5, height*5 });

	//Textures
	TextureManager textures;
	textures.addTexture("weapons_spritesheet.png");
	textures.addTexture("sample_spritesheet.png");
	textures.addTexture("box.png");
	textures.addTexture("box2.png");
	textures.addTexture("mob.png");
	textures.addTexture("bullet.png");

	//Hero and Weapons 
	Hero hero({ 500, 786 });
	//hero.showHitBox();
	int weaponIndex = 0;

	//Mobs
	std::vector<Mob *> mobs;
	mobs.push_back(new Mob(textures, { 700, 834 }, 100));
	mobs.push_back(new Mob(textures, { 1500, 900 }, 100));
	mobs.push_back(new Mob(textures, { 1000, 834 }, 100));
	mobs.push_back(new Mob(textures, { 300, 934 }, 100));

	//Level design
	std::vector<Block *> blocks;
	std::vector<std::pair<sf::Vector2u, float>> blockScript;
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 100, 300 }, 1.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 0, 300 }, 1.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 300, 300 }, 1.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 300, 0 }, 1.0f));

	blocks.push_back(new Block({ 0, 1000 } , { 10000, 64 }, "boxSprite.png"));
	blocks.push_back(new Block({ 0, 850 }, { 100, 100 }));
	blocks.push_back(new Block({ 1040, 900 }, { 1000, 64 }, "boxSprite.png"));	
	blocks.push_back(new Block({ 0, 300 }, { 64, 64 }, "boxSprite.png", &blockScript));

	sf::CircleShape s;
	sf::Texture d;
	d.loadFromFile("sprites/dud.png");
	s.setTexture(&d);
	s.setRadius(50);
	s.setPosition({ 100, 500 });
	sf::IntRect newpos;
	newpos.top = 500;
	newpos.left = 200;
	bool animate = false;
	float dx, dy = 5.0f;
	sf::Time animationdelay = sf::seconds(5.0f);
	float viewdx = 0;
	sf::View view(sf::FloatRect({ 0, 0, width, height }));
	hero.godMode(false);

	while (window.isOpen()) {
		// -- Events -- 
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) 
			animate = true;

		if (animate && (s.getPosition().x != newpos.left || s.getPosition().y != newpos.top)) {
			if (s.getPosition().x == newpos.left)
				dx = 0;
			else if (s.getPosition().x < newpos.left)
				dx = fabs(5.0f);
			else if (s.getPosition().x > newpos.left)
				dx = -fabs(5.0f);
			if (s.getPosition().y == newpos.top)
				dy = 0;
			else if (s.getPosition().y < newpos.top)
				dy = fabs(5.0f);
			else if (s.getPosition().y > newpos.top)
				dy = -fabs(5.0f);
			s.move({ dx, dy });
		}
		else
			animate = false;
			
	// -- Logic --
	//Gravity

	//Update
		if (!hero.dead())
			hero.update(cl.getElapsedTime().asSeconds(), view.getCenter(), blocks, mobs);
		else
			hero.deathAnimation(cl.getElapsedTime().asSeconds());
		//blocks[3]->update(cl.getElapsedTime().asSeconds());
		//std::cout << cl.getElapsedTime().asMicroseconds() << std::endl;
		cl.restart();

// -- Draw --

		window.clear();
		window.draw(background);

		//std::cout << view.getCenter().x << " " << view.getCenter().y << std::endl;
		if (hero.getPosition().x > (view.getCenter().x + (width / 12))) {
			viewdx = hero.getPosition().x - (view.getCenter().x + (width / 12));
			view.setCenter({ viewdx + view.getCenter().x, height / 2 });
		}
		else if (hero.getPosition().x < (view.getCenter().x - (width / 12))) {
			viewdx = (view.getCenter().x - (width / 12)) - hero.getPosition().x;
			if(view.getCenter().x - viewdx - (width / 2) > 0)
				view.setCenter({ view.getCenter().x - viewdx, height / 2 });
			else
				view.setCenter({ width / 2, height / 2 });
		}
		window.setView(view);

		//Environment
		for (Block * b : blocks)
			b->draw(window);
		window.draw(s);
		
		//Player
		hero.draw(window);

		for (Mob * a : mobs)
			a->draw(window);

		sf::View view2(window.getDefaultView());
		window.setView(view2);
		hero.drawHUD(window);
		
		window.display();
	}
	return 0;
}