#include "Guardian.h"

Guardian::Guardian(Hero &hero, TextureManager &textures) : guardianTextures(&textures) {
	level = 0;
	loyalty = 0;
	heroptr = &hero;

	nameFont.loadFromFile("fonts/arial.ttf");
	nameText.setCharacterSize(25);
	nameText.setFillColor(sf::Color::White);
	nameText.setOutlineColor(sf::Color::Black);
	nameText.setOutlineThickness(2.0f);
	nameText.setFont(nameFont);

	icon.setSize({ 127, 127 });
	icon.setOutlineThickness(3.0f);
	icon.setOutlineColor(sf::Color::Black);

	cd.setSize({ 127, 127 });

	//guardianSounds.addSound("sfx/");
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
	window.draw(cd);
	window.draw(nameText);
}

void Guardian::update(float time) {
	if (active && !ready) {
		elapsedcd += time;
		cd.setFillColor(sf::Color(224, 224, 224, 150));
		if (elapsedcd / cooldown < 1)
			cd.setSize({ 127, 127 * (elapsedcd / cooldown) });
		else {
			elapsedcd -= cooldown;
			active = false;
			ready = true;
		}
	}
	else {
		cd.setFillColor(sf::Color::Transparent);
	}
}

Estelle::Estelle(Hero &hero, TextureManager &textures) : Guardian(hero, textures) {
	name = "Estelle";
	nameText.setString(name);
	icon.setTexture(&guardianTextures->loadTexture("Estelle_TB.png"));
	icon.setPosition({ 1440, 33 });
	cd.setPosition({ 1440, 33 });
	nameText.setPosition({ 1469, 84});

	//activeSound.setBuffer();
}

Aiden::Aiden(Hero &hero, TextureManager &textures) : Guardian(hero, textures) {
	name = "Aiden";
	nameText.setString(name);
	icon.setTexture(&guardianTextures->loadTexture("Aiden_TB.png"));
	icon.setPosition({ 1599, 33 });
	cd.setPosition({ 1599, 33 });
	nameText.setPosition({ 1631, 84 });
}

Evangeline::Evangeline(Hero &hero, TextureManager &textures) : Guardian(hero, textures) {
	name = "Evangeline";
	nameText.setString(name);
	icon.setTexture(&guardianTextures->loadTexture("Evangeline_TB.png"));
	icon.setPosition({ 1758, 33 });
	cd.setPosition({ 1758, 33 });
	nameText.setPosition({ 1755, 84 });
}
