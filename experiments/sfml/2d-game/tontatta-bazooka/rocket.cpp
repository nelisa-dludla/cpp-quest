#include "rocket.h"

Rocket::Rocket(){}

Rocket::~Rocket(){}

void Rocket::init(std::string textureName, sf::Vector2f position, float speed){
	m_speed = speed;
	m_position = position;

	// Load texture
	m_texture.loadFromFile(textureName.c_str());
	// Create sprite and attach texture
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(position);
	m_sprite.setOrigin(m_texture.getSize().x / 2, m_texture.getSize().y / 2);
}

void Rocket::update(float dt){
	m_sprite.move(m_speed * dt, 0);
}

sf::Sprite Rocket::getSprite(){
	return m_sprite;
}
