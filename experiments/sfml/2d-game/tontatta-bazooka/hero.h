#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>

class Hero {
	public:
		Hero();
		~Hero();

		void init(std::string textureName, int frameCount, float animDuration, sf::Vector2f position, float mass);
		void update(float dt);
		void jump(float velocity);
		sf::Sprite getSprite();
	
	private:
		sf::Texture m_texture;
		sf::Sprite m_sprite;
		sf::Vector2f m_position;

		int jumpCount = 0;
		float m_mass;
		float m_velocity;
		const float m_gravity = 9.80f;
		bool m_grounded;

		int m_frameCount;
		float m_animDuration;
		float m_elapsedTime;
		sf::Vector2f m_spriteSize;
};

#endif
