#include "Hero.h"

Hero::Hero(sf::Vector2f newPos) : row(0){
	textures.addTexture("sample_spritesheet.png");
	heroTexture = textures.loadTexture("sample_spritesheet.png");
	heroSprite.setPosition(newPos);
	heroSprite.setTexture(heroTexture);
	hitbox.setSize({ 36, 63 });
	hitbox.setPosition(newPos.x + 14, newPos.y + 1);
	hitbox.setFillColor(sf::Color::Transparent);

	animation = new Animation(heroTexture, sf::Vector2u(4,16), 0.08f);
	faceRight = false;
	
	if (!nameFont.loadFromFile("fonts/arial.ttf"))
		std::cout << "Font not loaded" << std::endl;

	name.setString("Hero");
	name.setCharacterSize(18);
	name.setFont(nameFont);
	name.setFillColor(sf::Color::White);
	name.setOutlineColor(sf::Color::Black);
	name.setOutlineThickness(2);
	name.setStyle(sf::Text::Bold);
	name.setPosition({ 50, 25 });

	deathMessage.setFont(nameFont);
	deathMessage.setFillColor(sf::Color::White);
	deathMessage.setStyle(sf::Text::Bold);
	deathMessage.setPosition({ 75.0f, 80.0f });

	healthOverlay.setRadius(50.0f);
	healthOverlay.setPosition({ 50.0f, 50.0f });
	healthOverlay.setFillColor(sf::Color::Transparent);
	healthOverlay.setOutlineColor(sf::Color::Black);
	healthOverlay.setOutlineThickness(2);

	healthCircle.resize(hp);
	healthCircle.setPrimitiveType(sf::TrianglesFan);
	double angle = -pi / 2;
	for (size_t i = 0; i < maxhp; i++) {
		//x = center.x + radius*cos(t) , y = center.y + radius*sin(t)
		float x = 100.0f + 50.0f * std::cos(angle);
		float y = 100.0f + 50.0f * std::sin(angle);
		healthCircle[i].position = sf::Vector2f(x, y);
		healthCircle[i].color = sf::Color::Red;
		angle += pi / (maxhp / 2);
	}

	weapons.push_back(new Ranged("Gun", 10, { 0, 0, 64, 64 }, 600));
	weapons.push_back(new Melee("Sword", 10, { 0, 0, 64, 64 }, 0.5f));
	weapons.push_back(new Ranged("Machine Gun", 200, { 0, 0, 64,64 }, 500, 0.1f));
	weapon = weapons[0];

	guardians.push_back(new Estelle(*this));
	guardians.push_back(new Aiden(*this));
	guardians.push_back(new Evangeline(*this));
	gIndex = 0;
}

void Hero::drawHUD(sf::RenderWindow &window) {
	window.draw(name);
	window.draw(healthCircle);
	window.draw(healthOverlay);
	window.draw(deathMessage);
	weapon->drawHUD(window);
	guardians[gIndex]->draw(window);
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

	sf::Vector2f dist = blocks[3]->update(time);
	if (curr != -1 && blocks[curr]->moving)
		stop(dist);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		row = 8;
		animation->changeSwitchTime(0.03);
		if (moveSpeed <= maxSpeed && (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && !jumping)
			moveSpeed += 0.5f;
	}
	else {
		animation->changeSwitchTime(0.08);
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
			row = 13;
			jumpSpeed += gravity;
			move({ 0, jumpSpeed });
		}
	}

	if (curr != -1 && blocks[curr]->moving)
		stop(dist);

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		std::cout << "Guardian Index: " << gIndex << std::endl;
		if (gIndex < guardians.size() - 1)
			gIndex++;
		else
			gIndex = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		guardians[gIndex]->heroActive();
	}

	for (Mob * mob : mobs) 
		if (mob->collide(hitbox) && !mob->dead()) {
			changeHealth(-1);
			row = 3;
		}

	for (Weapon * w : weapons)
		w->update(time, mobs, getPosition());
	guardians[gIndex]->update(time);
	animation->update(row, time, faceRight);
	heroSprite.setTextureRect(animation->uvRect);
}

void Hero::wield(Weapon * w) {
	weapon = w;
}

Weapon* Hero::getWeapon() {
	return weapon;
}

void Hero::changeHealth(double x) { //HUD
	if (god)
		x = 0;
	if (x < -maxhp) {
		std::cout << "Damage cannot exceed available health" << std::endl;
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
			healthCircle[i].position = sf::Vector2f(100.0f, 100.0f);
		hpindex = temp;
	}
	else if (x > 0) {
		int temp = hpindex - x;
		if (temp < 0)
			temp = 0;
		double angle = -pi / 2;
		for (size_t i = 0; i < maxhp; i++) {
			if (i >= temp && i < hpindex) {
				float x = 100.0f + 50.0f * std::cos(angle);
				float y = 100.0f + 50.0f * std::sin(angle);
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

//pixel perfect test
/*bool Hero::PPcollide(Foreign &object) {
return Collision::PixelPerfectTest(heroSprite, object.getSprite());
}*/

//bounding test
bool Hero::BBcollide(const sf::Sprite &obj2) {
	return Collision::BoundingBoxTest(heroSprite, obj2);
}

//For Foreign Objects (does not use perfect pixel collision)
/*bool Hero::GBcollide(Foreign &object) {
if (delay <= 0) {
if (hitbox.getGlobalBounds().intersects(object.getGlobalBounds())) {
delay = cooldown;
return true;
}
return false;
}
return false;
}*/

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
	std::cout << "Estelle active" << std::endl;
	if (active)
		if (level == 0) {
			heroptr->changeHealth(15);
			active = false;
		}
		if (level == 1) {

		}
		if (level == 2) {

		}
		if (level == 3) {

		}
		if (level == 4) {

		}
}

void Aiden::heroActive() {
	/*
	Aiden's abilities are very inconsistent and troll, so they will
	only become crazier and more erratic the higher you level him up.
	*/
	std::cout << "Aiden active" << std::endl;
	if(active)
		if (level == 0) {
			heroptr->changeMaxSpeed(20.0f);
			active = false;
		}
}

void Evangeline::heroActive() {
	/*
	Evangeline's more reserved and in control, so she will enhance
	the hero's abilities the stronger you level her up as a reward.
	*/
	std::cout << "Evangeline active" << std::endl;
	if(active)
		if (level == 0) {
			if (heroptr->getWeapon()->isRanged())
				heroptr->getWeapon()->changeAttack(15.0f);
			else
				heroptr->getWeapon()->changeAttack(50.0f);
			active = false;
		}
}

//Passive Abilities
void Estelle::update(float time) {
	//Health regeneration
	Guardian::update(time);
	elapsed += time;
	if (elapsed >= 5.0f) {
		elapsed = 0.0f;
		heroptr->changeHealth(2.0);
	}
}

void Aiden::update(float time) {
	Guardian::update(time);

}

void Evangeline::update(float time) {
	Guardian::update(time);

}