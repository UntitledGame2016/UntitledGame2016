#include "Weapons.h"

Bullet::Bullet() { //No texture, default
	hitbox.setSize({ 20, 10 });
	hitbox.setFillColor(sf::Color::Red);
	hitbox.setPosition({ 5, -50 });

	bulletTexture.loadFromFile("sprites/bullet.png");
	bulletSprite.setTexture(bulletTexture);
}
Bullet::Bullet(TextureManager &textures, sf::IntRect bulletTexture) {
	hitbox.setSize({ 10, 10 });
	hitbox.setFillColor(sf::Color::Red);
	hitbox.setPosition({ -50, -50 });

	//Fix textures
}
void Bullet::draw(sf::RenderWindow& window) {
	window.draw(hitbox);
	window.draw(bulletSprite);
}
void Bullet::move(sf::Vector2f velocity) {
	hitbox.move(velocity);
	bulletSprite.move(velocity);
}
void Bullet::setPosition(sf::Vector2f pos) {
	hitbox.setPosition(pos);
	bulletSprite.setPosition(pos);
}
void Bullet::toggle(bool active) {
	fired = active;
}
bool Bullet::isready() {
	return fired;
}
float Bullet::getvel() {
	return velocity;
}
bool Bullet::collide(sf::Sprite &obj) {
	return Collision::BoundingBoxTest(bulletSprite, obj);
}
void Bullet::changeDir(bool i) {
	if (i && velocity < 0 || !i && velocity > 0)
		velocity *= -1;
}

sf::Vector2f Bullet::getHBSize() {
	return hitbox.getSize();
}

sf::Vector2f Bullet::getPosition() {
	return hitbox.getPosition();
}

Weapon::Weapon() {
	textures.addTexture("weapons_spritesheet.png");
	weaponTexture = textures.loadTexture("weapons_spritesheet.png");
	weaponSprite.setTexture(weaponTexture);
	weaponSprite.setTextureRect({ 0, 0, 64, 64 });
	durability = 10;

	name.setString("Fist");
}

Weapon::Weapon(std::string weaponName, int durability, sf::IntRect textureRect) : durability(durability) {
	textures.addTexture("weapons_spritesheet.png");
	weaponTexture = textures.loadTexture("weapons_spritesheet.png");
	weaponSprite.setTexture(weaponTexture);
	weaponSprite.setTextureRect(textureRect);
	weaponSprite.setPosition(500, 500);

	maxDurability = durability;

	if (!font.loadFromFile("fonts/arial.ttf"))
		std::cout << "Font not loaded" << std::endl;

	name.setFont(font);
	name.setString(weaponName);
	name.setCharacterSize(15);
	name.setFillColor(sf::Color::Black);
	name.setPosition({ 180, 20 });
}

void Weapon::draw(sf::RenderWindow &window) {
	window.draw(name);
	window.draw(weaponSprite);
}

Ranged::Ranged(std::string name, int durability, sf::IntRect textureRect, const float newfirerate) :
	Weapon(name, durability, textureRect), firerate(newfirerate) {

	clipText.setFont(font);
	clipText.setCharacterSize(15);
	clipText.setFillColor(sf::Color::Black);
	clipText.setPosition({ 300, 20 });

	for (int i = 0; i < durability; i++)
		bullets.push_back(new Bullet());
}

void Ranged::draw(sf::RenderWindow &window) {
	window.draw(clipText);
	Weapon::draw(window);
}

void Ranged::drawBullets(sf::RenderWindow &window) {
	for (size_t i = 0; i < bullets.size(); i++)
		bullets[i]->draw(window);
}


void Ranged::attack(sf::Vector2f pos, bool faceRight) {
	if (bullets.size() > 1 && delay <= 0) {
		delay = firerate;
		if (bulletindex < bullets.size()) {
			if (!faceRight && bullets[bulletindex]->getvel() > 0)
				bullets[bulletindex]->changeDir(false);
			else if (bullets[bulletindex]->getvel() < 0)
				bullets[bulletindex]->changeDir(true);
			bullets[bulletindex]->toggle(true);
			bullets[bulletindex]->setPosition({ pos.x + 32, pos.y + 32 });
			bulletsfired++;

			std::cout << "Bullets : " << bullets.size() - bulletsfired << std::endl;
			durability = bullets.size() - bulletsfired;
		}
		else {
			durability = 0;
			std::cout << "You're out of ammo." << std::endl;
		}
		bulletindex++;
	}
	else
		return;
}

void Ranged::update(float time, std::vector<Mob *> &mobs) {
	delay -= time;
	std::ostringstream temp;
	temp << durability;
	clipText.setString(temp.str());

	for (int i = bullets.size() - 1; i > -1; i--)
		if (bullets[i]->isready()) {
			bullets[i]->move({ bullets[i]->getvel(), 0 });
			if (bullets[i]->collide(mobs[0]->getSprite())) 
				mobs[0]->changeHealth(-5);
			if (bullets[i]->getPosition().x >= 1080 || bullets[i]->getPosition().x + bullets[i]->getHBSize().x <= 0)
				bullets[i]->toggle(false);
		}

}

void Ranged::reload(int newBullets) {
	delay = firerate;

	if (newBullets + (bullets.size() - bulletsfired) > maxDurability)
		newBullets = bullets.size() - bulletsfired;

	for (int i = 0; i < newBullets; i++)
		bullets.push_back(new Bullet());

	durability = bullets.size() - bulletsfired;
}