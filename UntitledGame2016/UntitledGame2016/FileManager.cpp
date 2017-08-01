#include "FileManager.h"

const void TextureManager::addTexture(const std::string &fileName) {
	sf::Texture* temp = new sf::Texture;
	if (!temp->loadFromFile("sprites/" + fileName)) {
		std::cout << "Cannot load texture " + fileName << std::endl;
		delete temp;
	}
	else {
		textureMap[fileName] = temp;
	}
}

const sf::Texture &TextureManager::loadTexture(const std::string &fileName) {
	std::map<std::string, sf::Texture *>::const_iterator searchTexture = textureMap.find(fileName);
	if (searchTexture != textureMap.end())
		return *searchTexture->second;
	else {
		std::cout << "Texture does not exist" << std::endl;
	}
}

const void AudioManager::addSound(const std::string &fileName) {
	sf::SoundBuffer* temp = new sf::SoundBuffer;
	if (!temp->loadFromFile("sfx/" + fileName)) {
		std::cout << "Cannot load sound " + fileName << std::endl;
		delete temp;
	}
	else {
		audioMap[fileName] = temp;
	}
}

const sf::SoundBuffer &AudioManager::loadSound(const std::string &fileName) {
	std::map<std::string, sf::SoundBuffer *>::const_iterator searchSound = audioMap.find(fileName);
	if (searchSound != audioMap.end())
		return *searchSound->second;
	else {
		std::cout << "Sound does not exist" << std::endl;
	}
}