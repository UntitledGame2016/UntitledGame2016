#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Hero.h"
#include "Weapons.h"
#include "Mobs.h"
#include "FileManager.h"

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
	textures.addTexture("sample_spritesheet.png");
	textures.addTexture("weapons_spritesheet.png");
	textures.addTexture("sample_spritesheet.png");
	textures.addTexture("box.png");
	textures.addTexture("box2.png");
	textures.addTexture("mob.png");
	textures.addTexture("bullet.png");
	textures.addTexture("Aiden_TB.png");
	textures.addTexture("Evangeline_TB.png");
	textures.addTexture("Estelle_TB.png");

	//Hero and Weapons 
	Hero hero({ 500, 786 }, textures);
	//hero.showHitBox();
	int weaponIndex = 0;

	//Mobs
	std::cout << "Spawning mobs..." << std::endl;
	std::vector<Mob *> mobs;
	mobs.push_back(new Mob(textures, { 700, 834 }, 100));
	mobs.push_back(new Mob(textures, { 400, 834 }, 100));
	mobs.push_back(new Mob(textures, { 1500, 834 }, 100));
	mobs.push_back(new Mob(textures, { 1000, 834 }, 100));
	mobs.push_back(new Mob(textures, { 300, 934 }, 100));
	mobs.push_back(new Mob(textures, { 1800, 834 }, 100));
	std::cout << "Mobs spawned." << std::endl;

	//Level design
	std::cout << "Creating level..." << std::endl;
	std::vector<Block *> blocks;
	std::vector<std::pair<sf::Vector2u, float>> blockScript;
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 500, 400 }, 2.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 1000, 900 }, 5.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 1000, 300 }, 5.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 100, 900 }, 0.8f));

	blocks.push_back(new Block({ 0, 1000 } , { 10000, 64 }, "boxSprite.png"));
	blocks.push_back(new Block({ 0, 850 }, { 100, 100 }));
	blocks.push_back(new Block({ 1040, 900 }, { 1000, 64 }, "boxSprite.png"));	
	blocks.push_back(new Block({ 500, 1000 }, { 64, 64 }, "boxSprite.png", &blockScript));

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
	std::cout << "Level complete." << std::endl;

	hero.godMode(true);

	sf::Music music;
	if (!music.openFromFile("songs/level01.ogg"))
		std::cout << "Error playing song." << std::endl;
	music.setLoop(true);
	music.setVolume(75);
	music.play();
	music.setPlayingOffset(sf::seconds(62));

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

		for (Mob * a : mobs) {
			//a->update(cl.getElapsedTime().asSeconds());
			a->draw(window);
		}

		sf::View view2(window.getDefaultView());
		window.setView(view2);
		hero.drawHUD(window);
		
		window.display();
	}
	return 0;
}