#include "Hero.h"

Hero::Hero(sf::Vector2f newPos) : row(0){
	textures.addTexture("sample_spritesheet.png");
	heroTexture = textures.loadTexture("sample_spritesheet.png");
	heroSprite.setPosition(newPos);
	heroSprite.setTexture(heroTexture);
	hitbox.setSize({ 36, 63 });
	hitbox.setPosition(newPos.x + 14, newPos.y + 1);
	hitbox.setFillColor(sf::Color::Transparent);

	animation = new Animation(heroTexture, sf::Vector2u(3,2), 1.0f);
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
}

void Hero::draw(sf::RenderWindow &window) {
	window.draw(heroSprite);
	window.draw(name);
	window.draw(hitbox);
	window.draw(healthCircle);
	window.draw(healthOverlay);
	window.draw(deathMessage);
}

void Hero::move(sf::Vector2f distance, float elapsed) {
	heroSprite.move(distance);
	hitbox.move(distance);
	if (distance.x > 0) {
		faceRight = true;
		row = 0;
	}
	if (distance.x < 0) {
		faceRight = false;
		row = 1;
	}
}

bool Hero::face() {
	return faceRight;
}

void Hero::update(float time, std::vector<Block *> blocks) {
	bool collisions = false;

	//Keys
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !jumping) {
		move({ 0, jumpSpeed }, time);
		jumping = true;
		for (size_t i = 0; i < blocks.size(); i++) 
			if (curr != i && blocks[i]->colliding(hitbox)) {
				std::cout << "Bottom Collision" << std::endl;
				jumping = false;
				jumpSpeed = 0;
				break;
			}
		curr = -1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		move({ moveSpeed, 0 }, time);
		if (fallLeft) {
			move({ -moveSpeed, 0 }, time);
		}
		if (!jumping && !blocks[curr]->colliding(hitbox)) {
			fallRight = true;
			jumpSpeed = 0;
			jumping = true;
		}
		for (size_t i = 0; i < blocks.size(); i++) {
			if (curr != i && blocks[i]->colliding(hitbox)) {
				std::cout << "Left Collision" << std::endl;
				move({ -moveSpeed, 0 }, time);
				break;
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		move({ -moveSpeed, 0 }, time);
		if (fallRight) {
			move({ moveSpeed, 0 }, time);
		}
		if (!jumping && !blocks[curr]->colliding(hitbox)) {
			fallLeft = true;
			jumpSpeed = 0;
			jumping = true;
		}
		for (size_t i = 0; i < blocks.size(); i++) {
			if (curr != i && blocks[i]->colliding(hitbox)) {
				std::cout << "Right Collision" << std::endl;
				move({ moveSpeed, 0 }, time);
				break;
			}
		}
	}

	if (fall && !jumping && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { //If falling
		if (getY() < blocks[curr]->getY()) {
			fallSpeed += gravity;
			move({ 0, fallSpeed }, time);
			for (size_t i = 0; i < blocks.size(); i++)
				if (blocks[i]->colliding(hitbox))
					setY(blocks[i]->getY());
		}
		else {
			fallSpeed = 0;
			fall = false;
		}
	}
	else if (jumping) {
		for (size_t i = 0; i < blocks.size(); i++) { //If landing
			float tempSpeed = jumpSpeed;
			move({ 0, jumpSpeed + gravity }, time);
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
					jumpSpeed = -15;
					jumping = false, fallLeft = false, fallRight = false;
					curr = i;
					collisions = true;
					break;
				}
			}
			move({ 0, -tempSpeed - gravity }, time);
		}
		if (!collisions) { //If still in the air
			jumpSpeed += gravity;
			move({ 0, jumpSpeed }, time);
		}
	}

	animation->update(row, time, faceRight);
	heroSprite.setTextureRect(animation->uvRect);
}

void Hero::changeHealth(const int x) { //HUD
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
	if (hp <= 0) {
		name.setString("You're dead LUL");
		deathMessage.setString("RIP");
		healthOverlay.setFillColor(sf::Color::Black);
	}
	std::cout << "Health: " << hp << std::endl;
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