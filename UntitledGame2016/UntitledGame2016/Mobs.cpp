#include "Mobs.h"

Mob::Mob(TextureManager &textures, sf::Vector2f newpos, int health, float newDamage) : 
	health(health), maxHealth(health), damage(newDamage) {
	hitbox.setSize({ 256, 256 });
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setPosition(newpos);

	barLength = hitbox.getSize().x*(3 / 2);
	healthbar.setSize({ barLength, 10 });
	healthbar.setFillColor(sf::Color::Red);
	healthbar.setOutlineColor(sf::Color::Black);
	healthbar.setOutlineThickness(2);

	texture = textures.loadTexture("mob.png");
	sprite.setPosition(hitbox.getPosition());
	sprite.setTexture(texture);

	mobSounds.addSound("mob_death.wav");
	mobDeath.setBuffer(mobSounds.loadSound("mob_death.wav"));
	mobDeath.setVolume(75);
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
	return Collision::BoundingBoxTest(sprite, obj);
}

bool Mob::collide(sf::RectangleShape &obj) {
	return hitbox.getGlobalBounds().intersects(obj.getGlobalBounds());
}

void Mob::update(float time) {
}

float Mob::getDamage() {
	return damage;
}

void Mob::changeHealth(const int hp) {
	health += hp;
	if (hp < 0)
		if (health < 0) {
			health = 0;
		}
	else 
		if (health > maxHealth)
			health = maxHealth;

	if(health == 0)
		mobDeath.play();
	float temp = barLength * (health / maxHealth);
	//std::cout << health << " " << maxHealth << " " << temp << " " << health / maxHealth << std::endl;
	healthbar.setSize({ temp, 10 });
}

bool Mob::dead() {
	if (health > 0) {
		return false;
	}
	return true;
}