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
	if (fired) {
		window.draw(hitbox);
		window.draw(bulletSprite);
	}
}
void Bullet::move(sf::Vector2f velocity) {
	hitbox.move(velocity);
	bulletSprite.move(velocity);
	if (hitbox.getPosition().x <= maxRange.x || hitbox.getPosition().x >= maxRange.y)
		fired = false;
}
void Bullet::setPosition(sf::Vector2f pos, float range) {
	hitbox.setPosition(pos);
	bulletSprite.setPosition(pos);
	maxRange.x = pos.x + hitbox.getSize().x - range;
	maxRange.y = pos.x + range;
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
void Bullet::changeFaceRight(bool i) {
	if (i) {
		velocity = fabs(velocity);
		bulletSprite.setTextureRect({ 0, 0, 20, 10 });
	}
	else {
		velocity = -fabs(velocity);
		bulletSprite.setTextureRect({20, 0, -20, 10});
	}
}

sf::Vector2f Bullet::getHBSize() {
	return hitbox.getSize();
}

sf::Vector2f Bullet::getPosition() {
	return hitbox.getPosition();
}

Weapon::Weapon() {
	durability = 10;

	name.setString("Fist");
}

Weapon::Weapon(std::string weaponName, int durability, float newDamage, sf::IntRect textureRect) : 
	durability(durability), maxDurability(durability), damage(newDamage) {
	
	if (!font.loadFromFile("fonts/arial.ttf"))
		std::cout << "Font not loaded" << std::endl;

	name.setFont(font);
	name.setString(weaponName);
	name.setCharacterSize(25);
	name.setFillColor(sf::Color::Black);
	name.setPosition({ 300, 75 });

	weaponSounds.addSound("m4.ogx");
	weaponSounds.addSound("m4_reload.wav");
	weaponSounds.addSound("miniuzi_9mm.wav");
	weaponSounds.addSound("miniuzi_9mm_reload.wav");
	weaponSounds.addSound("sword_unsheath.wav");
	weaponSounds.addSound("sword_hit01.wav");
	weaponSounds.addSound("sword_hit02.wav");
	weaponSounds.addSound("sword_hit03.wav");
	weaponSounds.addSound("sword_swing01.wav");
	weaponSounds.addSound("sword_swing02.wav");
	weaponSounds.addSound("sword_swing03.wav");
	weaponSounds.addSound("sword_repair.wav");
}

void Weapon::draw(sf::RenderWindow &window) {
	window.draw(name);
	//window.draw(weaponSprite);
}

void Weapon::drawHUD(sf::RenderWindow & window){
	
}

void Weapon::changeDamage(float newDmg) {
	damage = newDmg;
}

bool Weapon::isRanged() {
	return type;
}

void Weapon::playWield() {
	wieldSound.play();
}

float Weapon::getDamage() {
	return damage;
}

Ranged::Ranged(std::string name, int durability, sf::IntRect textureRect, float newDamage, const float newfirerate, const float range) :
	Weapon(name, durability, newDamage, textureRect), firerate(newfirerate), bulletRange(range) {

	if (newfirerate < 0.25f) {
		wieldSound.setBuffer(weaponSounds.loadSound("miniuzi_9mm_reload.wav"));
		attackSound.setBuffer(weaponSounds.loadSound("miniuzi_9mm.wav"));
		reloadSound.setBuffer(weaponSounds.loadSound("miniuzi_9mm_reload.wav"));
	}
	else {
		wieldSound.setBuffer(weaponSounds.loadSound("m4_reload.wav"));
		attackSound.setBuffer(weaponSounds.loadSound("m4.ogx"));
		reloadSound.setBuffer(weaponSounds.loadSound("m4_reload.wav"));
		attackSound.setVolume(50);
	}
	weaponDura.setFont(font);
	weaponDura.setCharacterSize(30);
	weaponDura.setFillColor(sf::Color::Black);
	weaponDura.setPosition({ 300, 100 });

	for (int i = 0; i < durability; i++)
		bullets.push_back(new Bullet());

	type = true;
	std::cout << "Wielding " << name << "." << std::endl;
}

void Ranged::draw(sf::RenderWindow &window) {
	for (size_t i = 0; i < bullets.size(); i++)
		if(bullets[i]->isready())
			bullets[i]->draw(window);
}

void Ranged::drawHUD(sf::RenderWindow &window) {
	Weapon::draw(window);
	window.draw(weaponDura);
}

void Ranged::attack(sf::Vector2f pos, bool faceRight) {
	if (bullets.size() > 1 && delay <= 0) {
		delay = firerate;
		if (durability > 0) {
			attackSound.play();
			if (faceRight)
				bullets[bulletindex]->changeFaceRight(true);
			else
				bullets[bulletindex]->changeFaceRight(false);
			if(!bullets[bulletindex]->isready())
				bullets[bulletindex]->toggle(true);
			bullets[bulletindex]->setPosition({ pos.x + 32, pos.y + 32 }, bulletRange);
			durability--;
			bulletsfired++;
			bulletindex++;
			if (bulletindex == bullets.size())
				bulletindex = 0;
		}
		else 
			durability == 0;
	}
	else
		return;
}

void Ranged::update(float time, std::vector<Mob *> &mobs, sf::Vector2f pos) {
	delay -= time;
	std::ostringstream temp;
	temp << durability;
	if (durability > 0)
		weaponDura.setString(temp.str());
	else
		weaponDura.setString("You're out of ammo.");
	for (int i = bullets.size() - 1; i > -1; i--)
		if (bullets[i]->isready()) {
			bullets[i]->move({ bullets[i]->getvel(), 0});
			for(Mob * mob : mobs)
				if (bullets[i]->collide(mob->getSprite()) && !mob->dead()) {
					bullets[i]->toggle(false);
					mob->changeHealth(-damage);
				}
		}
}

void Ranged::reload(int newBullets) {
	std::cout << maxDurability << " " << durability << std::endl;
	reloadSound.play();
	if (durability < maxDurability) {
		durability += newBullets;
		if (durability > maxDurability) {
			durability = maxDurability;
		}
	}
}

Melee::Melee() {
	hitbox.setSize({ 10,5 });
	hitbox.setPosition({ 0, 0 });
	hitbox.setFillColor(sf::Color::Yellow);
	attacking = false;

	weaponDura.setFont(font);
	weaponDura.setCharacterSize(30);
	weaponDura.setFillColor(sf::Color::Black);
	weaponDura.setPosition({ 300, 100 });

	damage = 25.0f;

	std::cout << "Wielding fist." << std::endl;
}

Melee::Melee(std::string name, int durability, sf::IntRect textureRect, float newDamage, float newSpeed, float range ) :
	Weapon(name, durability, newDamage, textureRect), range(range){
	hitbox.setSize({2, 15});
	hitbox.setPosition({ 0, 0 });
	hitbox.setFillColor(sf::Color::Transparent);
	attacking = false;

	wieldSound.setBuffer(weaponSounds.loadSound("sword_unsheath.wav"));
	swingSound.setBuffer(weaponSounds.loadSound("sword_swing01.wav"));
	attackSound.setBuffer(weaponSounds.loadSound("sword_hit01.wav"));
	reloadSound.setBuffer(weaponSounds.loadSound("sword_repair.wav"));
	
	weaponDura.setFont(font);
	weaponDura.setCharacterSize(30);
	weaponDura.setFillColor(sf::Color::Black);
	weaponDura.setPosition({ 300, 100 });

	/*if (attackSpeed < 5.0f)
		attackSpeed = 5.0f;
	else
		*/
	attackSpeed = newSpeed;

	type = false;
	std::cout << "Wielding " << name << "." << std::endl;
}

void Melee::update(float time, std::vector<Mob *> &mobs, sf::Vector2f pos) {
	delay -= time;

	float hbx = fabs(hitbox.getSize().x);
	float hby = hitbox.getSize().y;

	if (attacking) {
		if (fabs(hbx) >= range)
			attackSpeed = -fabs(attackSpeed);

		if (fabs(hbx) <= 0) {
			attackSpeed = fabs(attackSpeed);
			attacking = false;
		}

		hbx += attackSpeed;

		if (faceRight) {
			hbx = fabs(hbx);
			hitbox.setPosition({ pos.x + 64, pos.y + (64 + hitbox.getSize().y) / 2 });
		}
		else {
			hbx = -fabs(hbx);
			hitbox.setPosition({ pos.x, pos.y + (64 + hitbox.getSize().y) / 2 });
		}

		hitbox.setSize({ hbx, hby });
		for(Mob * mob : mobs)
			if (mob->collide(hitbox) && !mob->dead()) {
				randomSound = int(time * 10000) % 3;
				if (randomSound == 0) {
					attackSound.setBuffer(weaponSounds.loadSound("sword_hit01.wav"));
					swingSound.setBuffer(weaponSounds.loadSound("sword_swing01.wav"));
				}
				else if (randomSound == 1) {
					attackSound.setBuffer(weaponSounds.loadSound("sword_hit02.wav"));
					swingSound.setBuffer(weaponSounds.loadSound("sword_swing02.wav"));
				}
				else {
					attackSound.setBuffer(weaponSounds.loadSound("sword_hit03.wav"));
					swingSound.setBuffer(weaponSounds.loadSound("sword_swing03.wav"));
				}
				std::cout << time << " - > " << randomSound << std::endl;
				attackSound.play();
				mob->changeHealth(-damage);
				durability--;
				attacking = false;
			}
	}
	else {
		hitbox.setPosition({ 0, 0 });
		hitbox.setSize({ 5.0f, hby });
	}

	std::ostringstream temp;
	temp << durability;
	if (durability > 0)
		weaponDura.setString(temp.str());
	else
		weaponDura.setString("Your sword is broken.");
}

void Melee::attack(sf::Vector2f pos, bool face){
	faceRight = face;
	if (!attacking && durability > 0 && delay <= 0) {
		swingSound.play();
		delay = 0.2f;
		attacking = true;
	}
}

void Melee::showHitBox() {
	hitbox.setFillColor(sf::Color::Black);
}

bool Melee::isattacking() {
	return attacking;
}

void Melee::repair(int newDura) {
	reloadSound.play();
	durability += newDura;
	if (durability > maxDurability)
		durability = maxDurability;
}

void Melee::draw(sf::RenderWindow &window){
	window.draw(hitbox);
}

void Melee::drawHUD(sf::RenderWindow &window) {
	Weapon::draw(window);
	window.draw(weaponDura);
}

