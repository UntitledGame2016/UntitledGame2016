#include "Mobs.h"

Mob::Mob(TextureManager &textures, sf::Vector2f newpos, int health) : health(health), maxHealth(health) {
	hitbox.setSize({ 64, 64 });
	hitbox.setPosition(newpos);
	hitbox.setFillColor(sf::Color::Green);

	healthbar.setSize({ hitbox.getSize().x*(3 / 2), 10 });
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
	healthbar.setPosition({ hitbox.getPosition().x - hitbox.getSize().x*(3 / 4), hitbox.getPosition().y - 20 });
	window.draw(hitbox);
	window.draw(sprite);
	window.draw(healthbar);
}

bool Mob::collide(sf::Sprite &obj) {
	return Collision::PixelPerfectTest(sprite, obj);
}

void Mob::update(float time) {
	delay -= time;
}

void Mob::changeHealth(const int hp) {
	delay = cooldown;
	if (health - hp > 0 && hp < 0 && health > 0) {
		health += hp;
		float temp = hitbox.getSize().x * (health / maxHealth);
		std::cout << health << " " << temp << std::endl;
		healthbar.setSize({ temp, 64 });
	}
	else
		healthbar.setSize({ 0, 0 });

	//No increase health yet;
}