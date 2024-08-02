#ifndef ROCKET_H
#define ROCKET_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>

class Rocket {
	public:
		Rocket();
		~Rocket();

		void init(std::string textureName, sf::Vector2f position, float speed);
		void update(float dt);
		sf::Sprite getSprite();
	
	private:
		sf::Texture m_texture;
		sf::Sprite m_sprite;
		sf::Vector2f m_position;
		float m_speed;
};

#endif