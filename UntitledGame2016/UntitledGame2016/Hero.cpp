#include "Hero.h"

Hero::Hero(sf::Vector2f newPos, TextureManager &textures) : row(0), allTextures(&textures){
	std::cout << "Creating hero..." << std::endl;

	heroTexture = allTextures->loadTexture("sample_spritesheet.png");
	heroSprite.setPosition(newPos);
	heroSprite.setTexture(heroTexture);
	hitbox.setSize({ 36, 63 });
	hitbox.setPosition(newPos.x + 14, newPos.y + 1);
	hitbox.setFillColor(sf::Color::Transparent);

	animation = new Animation(heroTexture, sf::Vector2u(4,16), 0.09f);
	faceRight = false;
	
	if (!nameFont.loadFromFile("fonts/arial.ttf"))
		std::cout << "Font not loaded" << std::endl;

	name.setString("Hero");
	name.setCharacterSize(35);
	name.setFont(nameFont);
	name.setFillColor(sf::Color::White);
	name.setOutlineColor(sf::Color::Black);
	name.setOutlineThickness(2);
	name.setStyle(sf::Text::Bold);
	name.setPosition({ 50, 20 });

	deathMessage.setFont(nameFont);
	deathMessage.setFillColor(sf::Color::White);
	deathMessage.setStyle(sf::Text::Bold);
	deathMessage.setPosition({ 75, 80 });

	healthOverlay.setRadius(125);
	healthOverlay.setPosition({ 35, 60 });
	healthOverlay.setFillColor(sf::Color::Transparent);
	healthOverlay.setOutlineColor(sf::Color::Black);
	healthOverlay.setOutlineThickness(2);

	healthCircle.resize(hp);
	healthCircle.setPrimitiveType(sf::TrianglesFan);
	double angle = -pi / 2;
	for (size_t i = 0; i < maxhp; i++) {
		//x = center.x + radius*cos(t) , y = center.y + radius*sin(t)
		float x = healthOverlay.getPosition().x + healthOverlay.getRadius() + healthOverlay.getRadius()* std::cos(angle);
		float y = healthOverlay.getPosition().y + healthOverlay.getRadius() + healthOverlay.getRadius()* std::sin(angle);
		healthCircle[i].position = sf::Vector2f(x, y);
		healthCircle[i].color = sf::Color::Red;
		angle += pi / (maxhp / 2);
	}

	selected.setSize({ 127, 127 });
	selected.setPosition({ 1440, 33 });
	selected.setFillColor(sf::Color::Transparent);
	selected.setOutlineThickness(2.0f);
	selected.setOutlineColor(sf::Color::Yellow);

	//name, durability, intrect, damage, attackSpeed, range
	weapons.push_back(new Ranged("Gun", 10, { 0, 0, 64, 64 }, 5.0f, 0.25f, 500));
	weapons.push_back(new Melee("Sword", 10, { 0, 0, 64, 64 }, 15.0f, 5.0f));
	weapons.push_back(new Ranged("Machine Gun", 50, { 0, 0, 64,64 }, 10.0f, 0.1f, 750));
	weapons[1]->showHitBox();
	weapon = weapons[0];

	guardians.push_back(new Estelle(*this, textures));
	guardians.push_back(new Aiden(*this, textures));
	guardians.push_back(new Evangeline(*this, textures));
	gIndex = 0;

	std::cout << "Hero has spawned." << std::endl;
}

void Hero::drawHUD(sf::RenderWindow &window) {
	window.draw(name);
	window.draw(healthCircle);
	window.draw(healthOverlay);
	window.draw(deathMessage);
	weapon->drawHUD(window);
	for (Guardian * g : guardians)
		g->draw(window);
	window.draw(selected);
}

void Hero::draw(sf::RenderWindow &window) {
	window.draw(heroSprite);
	window.draw(hitbox);
	for (Weapon * w : weapons)
		w->draw(window);
}

void Hero::stop(sf::Vector2f distance) {
	heroSprite.move(distance);
	hitbox.move(distance);
}

void Hero::move(sf::Vector2f distance) {
	heroSprite.move(distance);
	hitbox.move(distance);
}

bool Hero::face() {
	return faceRight;
}

void Hero::update(float time, sf::Vector2f viewCoor, std::vector<Block *> blocks, std::vector<Mob *> &mobs) {
	bool collisions = false;
	row = 0;
	view.x = viewCoor.x - (1920 / 2);
	view.y = viewCoor.y - (1080 / 2);

	/*sf::Vector2f dist = blocks[3]->update(time);
	if (curr != -1 && blocks[curr]->moving)
		stop(dist);
	*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		row = 8;
		animation->changeSwitchTime(0.04);
		if (moveSpeed <= maxSpeed && (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && !jumping)
			moveSpeed += 0.5f;
	}
	else {
		animation->changeSwitchTime(0.09);
		if (moveSpeed > 3.5)
			moveSpeed -= 0.3f;
	}

	//Keys
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !jumping) {
		move({ 0, jumpSpeed });
		jumping = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			jumpRight = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			jumpLeft = true;
		for (size_t i = 0; i < blocks.size(); i++)
			if (curr != i && blocks[i]->colliding(hitbox)) {
				std::cout << "Bottom Collision" << std::endl;
				jumping = false;
				move({ 0, -jumpSpeed });
				break;
			}
		if (jumping)
			curr = -1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || jumpRight) {
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			row = 1;
		if (!weapon->isattacking())
			faceRight = true;
		move({ moveSpeed, 0 });
		if (curr != -1) {
			if (fallLeft && (blocks[curr]->getY() + blocks[curr]->getSize().y) >= hitbox.getPosition().y)
				move({ -moveSpeed, 0 });
			if (!jumping && !blocks[curr]->colliding(hitbox)) {
				fallRight = true;
				jumpSpeed = 0;
				jumping = true;
			}
		}
		for (size_t i = 0; i < blocks.size(); i++)
			if (curr != i && blocks[i]->colliding(hitbox)) {
				std::cout << "Left Collision" << std::endl;
				move({ -moveSpeed, 0 });
				break;
			}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || jumpLeft) {
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			row = 1;
		if (!weapon->isattacking())
			faceRight = false;
		move({ -moveSpeed, 0 });
		if (curr != -1) {
			if (fallRight && (blocks[curr]->getY() + blocks[curr]->getSize().y) >= hitbox.getPosition().y)
				move({ moveSpeed, 0 });
			if (!jumping && !blocks[curr]->colliding(hitbox)) {
				fallLeft = true;
				jumpSpeed = 0;
				jumping = true;
			}
		}
		for (size_t i = 0; i < blocks.size(); i++) {
			if (curr != i && blocks[i]->colliding(hitbox)) {
				std::cout << "Right Collision" << std::endl;
				move({ moveSpeed, 0 });
				break;
			}
		}
	}

	if (jumping) {
		for (size_t i = 0; i < blocks.size(); i++) { //If landing
			float tempSpeed = jumpSpeed;
			move({ 0, jumpSpeed + gravity });
			if (blocks[i]->colliding(hitbox)) {
				// If Hero collides but the bottom of the hero is still below the platform
				if (heroSprite.getPosition().y > blocks[i]->getY() + blocks[i]->getSize().y) {
					// sets the top of the hero to bottom of platform
					setY(blocks[i]->getY() + blocks[i]->getSize().y + 64);
					collisions = true;
					jumpSpeed = 0;
					break;
				}
				else {
					setY(blocks[i]->getY());
					jumpSpeed = -13;
					jumping = false, fallLeft = false, fallRight = false; jumpRight = false; jumpLeft = false;
					curr = i;
					collisions = true;
					break;
				}
			}
			move({ 0, -tempSpeed - gravity });
		}
		if (!collisions) { //If still in the air
			jumpSpeed += gravity;
			move({ 0, jumpSpeed });
		}
	}

	/*
	if (curr != -1 && blocks[curr]->moving)
		stop(dist);
	*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		weapon->attack(getPosition(), faceRight);
		row = 2 + weapon_index;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		wield(weapons[0]);
		weapon_index = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		wield(weapons[1]);
		weapon_index = 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
		wield(weapons[2]);
		weapon_index = 2;
	}

	changeWindow += time;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && changeWindow >= 0.12f) {
		changeWindow = 0;
		if (gIndex < guardians.size() - 1)
			gIndex++;
		else
			gIndex = 0;
		std::cout << "Current Guardian: " << guardians[gIndex]->getName() << std::endl;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && changeWindow >= 0.12f) {
		changeWindow = 0;
		if (gIndex > 0)
			gIndex--;
		else
			gIndex = guardians.size() - 1;
		std::cout << "Current Guardian: " << guardians[gIndex]->getName() << std::endl;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		guardians[gIndex]->heroActive();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && changeWindow >= 1.0f) {
		changeWindow = 0;
		if (weapon->isRanged())
			weapon->reload(10);
		else
			weapon->repair(10);
	}

	for (Mob * mob : mobs) 
		if (mob->collide(hitbox) && !mob->dead()) {
			damageWindow += time;
			if (damageWindow > 0.15f) {
				damageWindow = 0;
				changeHealth(-mob->getDamage());
			}
			row = 3;
		}

	selected.setPosition({ 1440 + float(159 * gIndex), 33 });

	for (Weapon * w : weapons)
		w->update(time, mobs, getPosition());
	for (Guardian * g : guardians)
		g->update(time);
	animation->update(row, time, faceRight);
	heroSprite.setTextureRect(animation->uvRect);
}

void Hero::wield(Weapon * w) {
	w->playWield();
	weapon = w;
}

float Hero::getDamage() {
	return weapon->getDamage();
}

Weapon* Hero::getWeapon() {
	return weapon;
}

void Hero::changeHealth(double x) { //HUD
	if (god)
		x = 0;
	if (x < -maxhp) {
		std::cout << "One hit attacks are not allowed." << std::endl;
		return;
	}
	hp += x;
	if (hp > maxhp)
		hp = maxhp;
	double angle = 0;
	if (x < 0) { //When taking damage
		int temp = hpindex + abs(x);
		if (temp > maxhp)
			temp = maxhp;
		for (size_t i = hpindex; i < temp; i++)
			healthCircle[i].position = sf::Vector2f(healthOverlay.getPosition().x + healthOverlay.getRadius(), 
				healthOverlay.getPosition().y + healthOverlay.getRadius());
		hpindex = temp;
	}
	else if (x > 0) {
		int temp = hpindex - x;
		if (temp < 0)
			temp = 0;
		double angle = -pi / 2;
		for (size_t i = 0; i < maxhp; i++) {
			if (i >= temp && i < hpindex) {
				float x = healthOverlay.getPosition().x + healthOverlay.getRadius() + healthOverlay.getRadius()* std::cos(angle);
				float y = healthOverlay.getPosition().y + healthOverlay.getRadius() + healthOverlay.getRadius()* std::sin(angle);
				healthCircle[i].position = sf::Vector2f(x, y);
			}
			angle += pi / (maxhp / 2);
		}
		hpindex = temp;
	}
	if (hp <= 0) { //Death
		name.setString("You died.");
		deathMessage.setString("RIP");
		deathMessage.setPosition({ view.x + 75, view.y + 80 });
		healthOverlay.setFillColor(sf::Color::Black);
		alive = false;
	}
	std::cout << "Health: " << hp << std::endl;
}

void Hero::godMode(bool toggle) {
	if (toggle)
		std::cout << "God mode: ON" << std::endl;
	else
		std::cout << "God mode: OFF" << std::endl;
	god = toggle;
}

bool Hero::dead() {
	if (!alive)
		return true;
	return false;
}

void Hero::deathAnimation(float time) {
	row = 10;
	animation->update(row, time, faceRight);
	heroSprite.setTextureRect(animation->uvRect);
	move({0, .5});
}

float Hero::getSpeed() {
	return moveSpeed;
}

void Hero::changeMaxSpeed(float newMaxSpeed) {
	maxSpeed = newMaxSpeed;
}

sf::Sprite Hero::getSprite() {
	return heroSprite;
}

//bounding test
bool Hero::BBcollide(const sf::Sprite &obj2) {
	return Collision::BoundingBoxTest(heroSprite, obj2);
}

sf::FloatRect Hero::getGlobalBounds() {
	return hitbox.getGlobalBounds();
}

sf::FloatRect Hero::getGlobalBounds2() {
	return heroSprite.getGlobalBounds();
}

void Hero::showHitBox() {
	hitbox.setFillColor(sf::Color::Blue);
}

sf::Vector2f Hero::getPosition() {
	return heroSprite.getPosition();
}

float Hero::getY() {
	return heroSprite.getPosition().y;
}

float Hero::getX() {
	return heroSprite.getPosition().x;
}
void Hero::setX(float position) {
	heroSprite.setPosition({ position,heroSprite.getPosition().y });
	hitbox.setPosition(heroSprite.getPosition().x + 14, position + 1);
}
void Hero::setY(float position) {
	heroSprite.setPosition({ heroSprite.getPosition().x, position });
	hitbox.setPosition(heroSprite.getPosition().x + 14, position + 1);
}


//Guardian Active abilities
void Estelle::heroActive() {
	/*
	Estelle's abilities are learned rather than upgraded because it is
	not her personality to want to hold back on great mechanics for the
	hero especially if she has them. So, old mechanics will become obsolete.
	*/

	if (ready && !active) {
		std::cout << "Estelle active" << std::endl;

		//Healing
		if (level == 0) {
			cooldown = 3.0f;
			heroptr->changeHealth(15);
			ready = false;
		}
		if (level == 1) {
			effectWindow = 5;
		}
		if (level == 2) {

		}
		active = true;
	}
}

void Aiden::heroActive() {
	/*
	Aiden's abilities are very inconsistent and troll, so they will
	only become crazier and more erratic the higher you level him up.
	*/
	if (ready && !active) {
		std::cout << "Aiden active" << std::endl;

		//Speed up
		if (level == 0) {
			heroptr->changeMaxSpeed(20.0f);
			ready = false;
		}
		//
		if (level == 1) {

		}
		//
		if (level == 2) {

		}
		active = true;
	}
}

void Evangeline::heroActive() {
	/*
	Evangeline's more reserved and in control, so she will enhance
	the hero's abilities the stronger you level her up as a reward.
	*/
	if (ready && !active){
		std::cout << "Evangeline active" << std::endl;

		//Increase damage of current weapon
		if (level == 0) {
			cooldown = 0;
			if (heroptr->getWeapon()->isRanged())
				heroptr->getWeapon()->changeDamage(15.0f);
			else
				heroptr->getWeapon()->changeDamage(50.0f);
			ready = false;
		}
		//
		if (level == 1) {

		}
		//
		if (level == 2) {

		}
		active = true;
	}
}

//Passive Abilities
void Estelle::update(float time) {
	//Health regeneration
	Guardian::update(time);
	elapsed += time;
	if (elapsed >= 5.0f && level == 0) {
		elapsed -= 5.0f;
		heroptr->changeHealth(2.0);
	}

	//Lifesteal
	if (level == 1) {
		if (effectWindow <= 5.0f) {

		}
	}

}

void Aiden::update(float time) {
	Guardian::update(time);

	//Aiden passive
}

void Evangeline::update(float time) {
	Guardian::update(time);

	//Evangeline passive
}