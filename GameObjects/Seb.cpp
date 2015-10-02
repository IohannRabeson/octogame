#include "Seb.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

Seb::Seb(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(SEB_OSS, scale, position, 5u, 0.2f),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(8.f)),
	m_state(Idle)
{
	//	octo::ResourceManager & resources = octo::Application::getResourceManager();

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.2f), 5u);
	frames.emplace_back(sf::seconds(0.2f), 6u);
	frames.emplace_back(sf::seconds(0.2f), 7u);
	frames.emplace_back(sf::seconds(0.2f), 8u);
	frames.emplace_back(sf::seconds(0.2f), 9u);
	frames.emplace_back(sf::seconds(0.2f), 5u);
	frames.emplace_back(sf::seconds(0.2f), 6u);
	frames.emplace_back(sf::seconds(0.2f), 7u);
	frames.emplace_back(sf::seconds(0.2f), 8u);
	frames.emplace_back(sf::seconds(0.2f), 9u);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::NoLoop);
}

void Seb::update(sf::Time frameTime)
{
	InstanceDecor::update(frameTime);
	if (m_state == Idle)
	{
		m_timer += frameTime;
		if (m_timer > m_timerMax)
		{
			m_timer = sf::Time::Zero;
			m_state = Special;
			getSprite().setAnimation(m_animation);
		}
	}
	else if (m_state == Special)
	{
		m_timer += frameTime;
		if (m_timer >= sf::seconds(2.f))
		{
			m_timer = sf::Time::Zero;
			getSprite().setAnimation(getAnimation());
			m_state = Idle;
		}
	}
}
