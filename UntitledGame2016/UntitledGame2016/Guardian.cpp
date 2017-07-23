#include "Guardian.h"

Guardian::Guardian(Hero &hero) {
	level = 0;
	loyalty = 0;
	heroptr = &hero;
	active = true;

	icon.setPosition({ 175, 50 });
	icon.setRadius(50.0f);
	icon.setOutlineThickness(3.0f);
	icon.setOutlineColor(sf::Color::Black);
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
}

void Guardian::update(float time) {
	if (!active) {
		icon.setFillColor(sf::Color::Black);
	}
}

Estelle::Estelle(Hero &hero) : Guardian(hero) {
	icon.setFillColor(sf::Color::Red);
}

Aiden::Aiden(Hero &hero) : Guardian(hero) {
	icon.setFillColor(sf::Color::Yellow);
}

Evangeline::Evangeline(Hero &hero) : Guardian(hero) {
	icon.setFillColor(sf::Color::Green);
}
