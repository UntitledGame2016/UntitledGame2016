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

	//Textures
	TextureManager textures;
	textures.addTexture("sample_spritesheet.png");
	textures.addTexture("sample_spritesheet.png");
	textures.addTexture("box.png");
	textures.addTexture("box2.png");
	textures.addTexture("mob.png");
	textures.addTexture("bullet.png");
	textures.addTexture("Aiden_TB.png");
	textures.addTexture("Evangeline_TB.png");
	textures.addTexture("Estelle_TB.png");
	textures.addTexture("bg.jpg");

	//Background
	sf::RectangleShape background;
	background.setTexture(&textures.loadTexture("bg.jpg"));
	background.setSize({ 10000, height});
	
	//Hero and Weapons 
	Hero hero({ 500, 786 }, textures);
	//hero.showHitBox();
	int weaponIndex = 0;

	//Mobs
	std::cout << "Spawning mobs..." << std::endl;
	std::vector<Mob *> mobs;
	mobs.push_back(new Mob(textures, { 5000, 744 }, 100));
	mobs.push_back(new Mob(textures, { 4000, 744 }, 100));
	mobs.push_back(new Mob(textures, { 1500, 644 }, 100));
	mobs.push_back(new Mob(textures, { 1000, 644 }, 100));
	mobs.push_back(new Mob(textures, { 300, 744 }, 100));
	mobs.push_back(new Mob(textures, { 1800, 644 }, 100, 10.0f));
	std::cout << "Mobs spawned." << std::endl;

	//Level design
	std::cout << "Creating level..." << std::endl;
	std::vector<Block *> blocks;
	std::vector<std::pair<sf::Vector2u, float>> blockScript;
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 500, 400 }, 2.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 1000, 900 }, 5.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 1000, 300 }, 5.0f));
	blockScript.push_back(std::pair<sf::Vector2u, float>({ 100, 900 }, 0.8f));

	blocks.push_back(new Block({ 0, 1000 } , { 15000, 64 }, "grass.png"));
	blocks.push_back(new Block({ 0, 850 }, { 100, 100 }));
	blocks.push_back(new Block({ 1040, 900 }, { 1000, 64 }, "grass.png"));	
	//blocks.push_back(new Block({ 500, 1000 }, { 64, 64 }, "boxSprite.png", &blockScript));

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
	sf::View playerView(sf::FloatRect({ 0, 0, width, height }));
	std::cout << "Level complete." << std::endl;

	hero.godMode(false);

	sf::Music music;
	if (!music.openFromFile("songs/level01.ogg"))
		std::cout << "Error playing song." << std::endl;
	music.setLoop(true);
	music.setVolume(75);
	music.play();
	//music.setPlayingOffset(sf::seconds(62));

	sf::Text instructions;
	instructions.setString("Controls: \nWASD - Movement\nLShift - Run\nA - Attack\nNUM1-3 - Weapons\nR - Reload\nS/D - Toggle Guardian\nSPACE - Guardian Active");
	instructions.setPosition({ 750, 25 });
	sf::Font font;
	font.loadFromFile("fonts/arial.ttf");
	instructions.setFont(font);
	instructions.setFillColor(sf::Color::Black);
	instructions.setCharacterSize(25);

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
			hero.update(cl.getElapsedTime().asSeconds(), playerView.getCenter(), blocks, mobs);
		else
			hero.deathAnimation(cl.getElapsedTime().asSeconds());
		//blocks[3]->update(cl.getElapsedTime().asSeconds());
		//std::cout << cl.getElapsedTime().asMicroseconds() << std::endl;
		cl.restart();

// -- Draw --

		window.clear();

		//std::cout << view.getCenter().x << " " << view.getCenter().y << std::endl;
		if (hero.getPosition().x > (playerView.getCenter().x + (width / 12))) {
			viewdx = hero.getPosition().x - (playerView.getCenter().x + (width / 12));
			playerView.setCenter({ viewdx + playerView.getCenter().x, height / 2 });
		}
		else if (hero.getPosition().x < (playerView.getCenter().x - (width / 12))) {
			viewdx = (playerView.getCenter().x - (width / 12)) - hero.getPosition().x;
			if(playerView.getCenter().x - viewdx - (width / 2) > 0)
				playerView.setCenter({ playerView.getCenter().x - viewdx, height / 2 });
			else
				playerView.setCenter({ width / 2, height / 2 });
		}
		window.setView(playerView);

		//Environment
		window.draw(background);
		for (Block * b : blocks)
			b->draw(window);
		window.draw(s);
		
		//Player
		hero.draw(window);

		for (Mob * a : mobs) {
			//a->update(cl.getElapsedTime().asSeconds());
			a->draw(window);
		}

		sf::View hudView(window.getDefaultView());
		window.setView(hudView);
		hero.drawHUD(window);
		
		window.draw(instructions);

		window.display();
	}
	return 0;
}