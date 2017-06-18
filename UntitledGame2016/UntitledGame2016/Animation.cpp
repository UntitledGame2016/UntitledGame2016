#include "Animation.h"

Animation::Animation(sf::Texture &texture, sf::Vector2u imageCount, float switchTime):
		imageCount(imageCount), switchTime(switchTime){
	totalTime = 0.0f;
	currentImage.x = 0;
}

void Animation::update(int row, float deltaTime, bool faceRight) {
	
	currentImage.y = row;
	totalTime += deltaTime;

	if (totalTime >= switchTime) {
		totalTime -= switchTime;
		currentImage.x++;
		if (currentImage.x >= imageCount.x)
			currentImage.x = 0;
	}

	uvRect.left = currentImage.x * uvRect.width;
	uvRect.top = currentImage.y * uvRect.height;

	if (faceRight) {
		uvRect.left = currentImage.x * uvRect.width;
		uvRect.width = abs(uvRect.width);
	}
	else {
		uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
		uvRect.width = -abs(uvRect.width);
	}
}