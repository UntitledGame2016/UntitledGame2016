#include "Guardian.h"

Guardian::Guardian(Hero &hero, TextureManager &textures) : guardianTextures(&textures) {
	level = 0;
	loyalty = 0;
	heroptr = &hero;
	active = true;

	nameFont.loadFromFile("fonts/arial.ttf");
	nameText.setCharacterSize(30);
	nameText.setFillColor(sf::Color::Black);
	nameText.setPosition({ 400, 45 });
	nameText.setFont(nameFont);

	icon.setPosition({ 200, 30 });
	icon.setRadius(75.0f);
	icon.setOutlineThickness(3.0f);
	icon.setOutlineColor(sf::Color::Black);
}

std::string Guardian::getName() {
	return name;
}

void Guardian::levelUp(int inc = 1) {
	if(level < 5)
		level += inc;
}

void Guardian::move(sf::Vector2f distance) {
	icon.move(distance);
}

void Guardian::setPosition(sf::Vector2f pos) {
	icon.setPosition(pos);
}

sf::Vector2f Guardian::getPosition() {
	return icon.getPosition();
}

void Guardian::draw(sf::RenderWindow &window) {
	window.draw(icon);
	window.draw(nameText);
}

void Guardian::update(float time) {
	if (!active) {
		icon.setFillColor(sf::Color::Black);
	}
}

Estelle::Estelle(Hero &hero, TextureManager &textures) : Guardian(hero, textures) {
	name = "Estelle";
	nameText.setString(name);
	guardianTexture = guardianTextures->loadTexture("Estelle_TB.png");
	icon.setTexture(&guardianTexture);
	//icon.setFillColor(sf::Color::Red);
}

Aiden::Aiden(Hero &hero, TextureManager &textures) : Guardian(hero, textures) {
	name = "Aiden";
	nameText.setString(name);
	guardianTexture = guardianTextures->loadTexture("Aiden_TB.png");
	icon.setTexture(&guardianTexture);
	//icon.setFillColor(sf::Color::Yellow);
}

Evangeline::Evangeline(Hero &hero, TextureManager &textures) : Guardian(hero, textures) {
	name = "Evangeline";
	nameText.setString(name);
	guardianTexture = guardianTextures->loadTexture("Evangeline_TB.png");
	icon.setTexture(&guardianTexture);
	//icon.setFillColor(sf::Color::Green);
}
