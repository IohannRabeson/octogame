#include "Concert.hpp"
#include <Interpolations.hpp>
#include <SFML/Graphics.hpp>
#include <Application.hpp>
#include <Camera.hpp>

Concert::Concert(void) :
	SimpleObject(CONCERT_OSS),
	m_generator("random"),
	m_particlesCount(20),
	m_particles(new MusicSystem[m_particlesCount])
{
	typedef octo::SpriteAnimation::Frame	Frame;
	setupAnimation({Frame(sf::seconds(0.2f), 0u),
					Frame(sf::seconds(0.2f), 1u),
					Frame(sf::seconds(0.2f), 2u),
					Frame(sf::seconds(0.2f), 3u)}, octo::LoopMode::Loop);

	setupBox(this, static_cast<std::size_t>(GameObjectType::Concert), static_cast<std::size_t>(GameObjectType::Player));
	getSprite().setScale(sf::Vector2f(0.8f, 0.8f));
	for (std::size_t i = 0; i < m_particlesCount; i++)
		m_particles[i].canEmit(true);
}

void Concert::update(sf::Time frameTime)
{
	SimpleObject::update(frameTime);
	for (std::size_t i = 0; i < m_particlesCount; i++)
		m_particles[i].update(frameTime);
}

void Concert::setPosition(sf::Vector2f const & position)
{
	sf::Vector2f const & size = getSprite().getGlobalSize();

	SimpleObject::setPosition(position - sf::Vector2f(0.f, size.y - 100.f));
	for (std::size_t i = 3; i < m_particlesCount; i++)
	{
		sf::Vector2f randomPos(m_generator.randomFloat(0.f, size.x), -m_generator.randomFloat(0.f, size.x));
		m_particles[i].setEmitter(position + randomPos);
	}
}

void Concert::drawFront(sf::RenderTarget & render, sf::RenderStates) const
{
	for (std::size_t i = 0; i < m_particlesCount; i++)
		m_particles[i].draw(render);
}
