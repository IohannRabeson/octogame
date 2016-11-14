#include "Concert.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>
#include <SFML/Graphics.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
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
		
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resource = octo::Application::getResourceManager();

	m_sound = audio.playSound(resource.getSound(MUSIC_ACTION_FAST_OGG), 0.f);
	if (m_sound)
		m_sound->setLoop(true);
}

void Concert::update(sf::Time frameTime)
{
	SimpleObject::update(frameTime);

	sf::Vector2f const & octoPosition = Progress::getInstance().getOctoPos();
	sf::Vector2f const & position = getSprite().getPosition() + getSprite().getGlobalSize() / 2.f;
	float dist = std::sqrt(std::pow(position.x - octoPosition.x, 2u) + std::pow(position.y - octoPosition.y, 2u));

	if (m_sound)
	{
		float volume = std::min(1.f - dist / 2000.f, 1.f);
		m_sound->setVolume(volume * Progress::getInstance().getMusicVolume());
	}

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
