#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include "hero.h"
#include "enemy.h"
#include "rocket.h"

sf::Vector2f viewSize(1024, 768);
sf::VideoMode vm(viewSize.x, viewSize.y);
sf::RenderWindow window(vm, "Tontatta Bazooka", sf::Style::Default);
// Sky sprite
sf::Texture skyTexture;
sf::Sprite skySprite;
// Bg sprite
sf::Texture bgTexture;
sf::Sprite bgSprite;
// Hero
Hero hero;
// Enemies
std::vector<Enemy*> enemies;
// Time
float currentTime;
float prevTime = 0.0f;
// Rockets
std::vector<Rocket*> rockets;
// Score
int score = 0;
// GameOver?
bool gameover = true;
// Text
sf::Font headingFont;
sf::Text headingText;

sf::Font scoreFont;
sf::Text scoreText;

sf::Text tutorialText;
// Audio
sf::Music bgMusic;

sf::SoundBuffer fireBuffer;
sf::SoundBuffer hitBuffer;

sf::Sound fireSound(fireBuffer);
sf::Sound hitSound(hitBuffer);

void init();
void draw();
void updateInput();
void update(float dt);
void spawnEnemy();
void shoot();
bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2);
void reset();

int main() {
	sf::Clock clock;
	init();

	while (window.isOpen()) {
		updateInput();

		sf::Time dt = clock.restart();
		// Check if gameover
		if (!gameover) {
			update(dt.asSeconds());
		}
		window.clear(sf::Color::Red);
		draw();

		window.display();
	}

	return 0;
}

void init(){
	// Audio
	// Background music
	bgMusic.openFromFile("assets/audio/bgMusic.ogg");
	bgMusic.play();
	// Sounds
	hitBuffer.loadFromFile("assets/audio/hit.ogg");
	fireBuffer.loadFromFile("assets/audio/fire.ogg");
	// Load Textures
	skyTexture.loadFromFile("assets/graphics/sky.png");
	bgTexture.loadFromFile("assets/graphics/bg.png");
	// Set and attach Textures to Sprites
	// skey
	skySprite.setTexture(skyTexture);
	// bg
	bgSprite.setTexture(bgTexture);
	// Load font
	headingFont.loadFromFile("assets/fonts/SnackerComic.ttf");
	scoreFont.loadFromFile("assets/fonts/arial.ttf");
	// Heading text
	headingText.setFont(headingFont);
	headingText.setString("Tontatta Bazooka");
	headingText.setCharacterSize(84);
	headingText.setFillColor(sf::Color::Black);

	sf::FloatRect headingbounds = headingText.getGlobalBounds();
	headingText.setOrigin(headingbounds.width / 2, headingbounds.height / 2);
	headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));
	// Score text
	scoreText.setFont(scoreFont);
	scoreText.setString("Score: 0");
	scoreText.setCharacterSize(45);
	scoreText.setFillColor(sf::Color::Black);

	sf::FloatRect scorebounds = scoreText.getGlobalBounds();
	scoreText.setOrigin(scorebounds.width / 2, scorebounds.height / 2);
	scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));
	// Tutorial text
	tutorialText.setFont(scoreFont);
	tutorialText.setString("Press Space to Fire and Start Game, Up Arrow to Jump");
	tutorialText.setCharacterSize(35);
	tutorialText.setFillColor(sf::Color::Black);

	sf::FloatRect tutorialbounds = tutorialText.getGlobalBounds();
	tutorialText.setOrigin(tutorialbounds.width / 2, tutorialbounds.height / 2);
	tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.20f));

	// hero
	hero.init("assets/graphics/heroAnim.png", 4, 1.0f, sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f), 200);

	srand((int)time(0));
}

void draw(){
	window.draw(skySprite);
	window.draw(bgSprite);
	window.draw(hero.getSprite());
	// Draw enemies
	for (Enemy* enemy : enemies){
		window.draw(enemy->getSprite());
	}
	// Draw rockets
	for (Rocket* rocket : rockets){
		window.draw(rocket->getSprite());
	}
	// Text
	if (gameover) {
		window.draw(headingText);
		window.draw(tutorialText);
	} else {
		window.draw(scoreText);
	}
}


void updateInput(){
	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Up) {
				hero.jump(750.0f);
			}
			if (event.key.code == sf::Keyboard::Space) {
				if (gameover) {
					gameover = false;
					reset();
				} else {
					shoot();
				}
			}
		}
		if (event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed) {
			window.close();
		}

	}
}

void update(float dt){
	hero.update(dt);
	
	currentTime += dt;
	// Spawn enemies
	if (currentTime >= prevTime + 1.125f){
		spawnEnemy();
		prevTime = currentTime;
	}
	// Update enemies
	for (int i = 0; i < enemies.size(); i++){
		Enemy* enemy = enemies[i];

		enemy->update(dt);

		if (enemy->getSprite().getPosition().x < 0) {
			enemies.erase(enemies.begin());
			delete(enemy);
			gameover = true;
		}
	}
	// Update rockets
	for (int i = 0; i < rockets.size(); i++){
		Rocket* rocket = rockets[i];

		rocket->update(dt);

		if (rocket->getSprite().getPosition().x > viewSize.x) {
			rockets.erase(rockets.begin() + i);
			delete(rocket);
		}
	}
	// Check for collision between rocket and enemies
	for (int i = 0; i < rockets.size(); i++){
		for (int j = 0; j < enemies.size(); j++){
			Rocket* rocket = rockets[i];
			Enemy* enemy = enemies[j];

			if (checkCollision(rocket->getSprite(), enemy->getSprite())) {
				hitSound.play();
				// Update score
				score++;

				std::string finalScore = "Score: " + std::to_string(score);
				scoreText.setString(finalScore);
				sf::FloatRect scorebounds = scoreText.getLocalBounds();
				scoreText.setOrigin(scorebounds.width / 2, scorebounds.height / 2);
				scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

				rockets.erase(rockets.begin() + i);
				enemies.erase(enemies.begin() + j);

				delete rocket;
				delete enemy;

				std::cout << "Rocket intersects enemy" << std::endl;
			}
		}
	}
}


void spawnEnemy(){
	int randLoc = rand() % 3;
	sf::Vector2f enemyPos;
	float speed;

	switch (randLoc) {
		case 0:
			enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.75f);
			speed = -400;
			break;
		case 1:
			enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.60f);
			speed = -550;
			break;
		case 2:
			enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.40f);
			speed = -650;
			break;
		default:
			std::cout << "Incorrect y value" << std::endl;
			return;
	}

	Enemy* enemy = new Enemy();
	enemy->init("assets/graphics/enemy.png", enemyPos, speed);

	enemies.push_back(enemy);
}

void shoot(){
	Rocket* rocket = new Rocket();

	rocket->init("assets/graphics/rocket.png", hero.getSprite().getPosition(), 400.0f);
	
	rockets.push_back(rocket);
	fireSound.play();
}

bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2){
	sf::FloatRect shape1 = sprite1.getGlobalBounds();
	sf::FloatRect shape2 = sprite2.getGlobalBounds();

	if (shape1.intersects(shape2)) {
		return true;
	}
	else {
	return false;
	}
}

void reset(){
	score = 0;
	currentTime = 0.0f;
	prevTime = 0.0;
	scoreText.setString("Score: 0");

	for (Enemy* enemy : enemies){
		delete(enemy);
	}

	for (Rocket* rocket : rockets){
		delete(rocket);
	}

	enemies.clear();
	rockets.clear();
}
