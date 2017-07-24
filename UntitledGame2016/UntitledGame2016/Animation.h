#include <SFML/Graphics.hpp>

class Animation {
	float switchTime;
	float totalTime;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
public:
	sf::IntRect uvRect;
	Animation(sf::Texture &texture, sf::Vector2u imageCount, float switchTime);
	void update(int row, float deltaTime, bool faceRight);
	void changeSwitchTime(float time);
};