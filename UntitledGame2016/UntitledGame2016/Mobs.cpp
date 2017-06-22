#include "Mobs.h"

Mob::Mob(TextureManager &textures, sf::Vector2f newpos, int health) : health(health), maxHealth(health) {
	hitbox.setSize({ 64, 64 });
	hitbox.setPosition(newpos);
	hitbox.setFillColor(sf::Color::Green);

	barLength = hitbox.getSize().x*(3 / 2);
	healthbar.setSize({ barLength, 10 });
	healthbar.setFillColor(sf::Color::Red);
	healthbar.setOutlineColor(sf::Color::Black);
	healthbar.setOutlineThickness(2);

	texture = textures.loadTexture("mob.png");
	sprite.setPosition(hitbox.getPosition());
	sprite.setTexture(texture);
}

void Mob::setPosition(sf::Vector2f pos) {
	sprite.setPosition(pos);
	hitbox.setPosition(pos);
}

void Mob::draw(sf::RenderWindow &window) {
	if (health > 0) {
		healthbar.setPosition({ hitbox.getPosition().x - hitbox.getSize().x*(3 / 4), hitbox.getPosition().y - 20 });
		window.draw(hitbox);
		window.draw(sprite);
		window.draw(healthbar);
	}
}

bool Mob::collide(sf::Sprite &obj) {
	return Collision::PixelPerfectTest(sprite, obj);
}

void Mob::update(float time) {
	//
}

void Mob::changeHealth(const int hp) {
	health += hp;
	if (hp < 0)
		if (health < 0)
			health = 0;
	else 
		if (health > maxHealth)
			health = maxHealth;
	float temp = barLength * (health / maxHealth);
	std::cout << health << " " << maxHealth << " " << temp << " " << health / maxHealth << std::endl;
	healthbar.setSize({ temp, 10 });
}

bool Mob::dead() {
	if (health > 0)
		return false;
	return true;
}