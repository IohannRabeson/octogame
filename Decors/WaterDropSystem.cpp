#include "WaterDropSystem.hpp"
#include <Math.hpp>
#include <Interpolations.hpp>
#include <ctime>

WaterDropSystem::WaterDropSystem() :
	m_engine(std::time(0)),
	m_creationTimeDistri(0.1f, 0.2f),
	m_lifeTimeDistri(0.6f, 0.8f),
	m_directionDistri(octo::PiDiv2, octo::Pi + octo::PiDiv2),
	m_widthDistri(0.f, 1.f),
	m_timer(sf::Time::Zero),
	m_nextCreation(sf::Time::Zero),
	m_timerDuration(sf::Time::Zero),
	m_timerDurationMax(sf::seconds(1.f)),
	m_color(sf::Color(28, 172, 228, 110)),
	m_canEmit(false),
	m_width(100.f)
{
	static float const		Size = 5.f;
	WaterDropSystem::Prototype	prototype;

	prototype.emplace_back(sf::Vertex({-Size, Size}));
	prototype.emplace_back(sf::Vertex({Size, -Size}));
	prototype.emplace_back(sf::Vertex({-Size, -Size}));
	reset(prototype, sf::Triangles, 2000);
}

void	WaterDropSystem::canEmit(bool canEmit)
{
	m_canEmit = canEmit;
}

void	WaterDropSystem::setColor(sf::Color const & color)
{
	m_color = color;
}

void	WaterDropSystem::setEmitter(sf::Vector2f const & emitter)
{
	m_emitter = emitter;
}

void	WaterDropSystem::setWidth(float width)
{
	m_width = width;
}

void	WaterDropSystem::update(sf::Time frameTime)
{
	ParticleSystem::update(frameTime);

	m_timer += frameTime;
	while (m_timer > m_nextCreation)
	{
		if (m_canEmit)
		{
			float posX = m_widthDistri(m_engine) * m_width;
			emplace(m_color, m_emitter + sf::Vector2f(posX, 0.f), sf::Vector2f(1.f, 1.f), m_directionDistri(m_engine) * 180,
					sf::Time::Zero,
					sf::seconds(m_lifeTimeDistri(m_engine)));
		}
		m_timer -= m_nextCreation;
		m_nextCreation = sf::seconds(m_creationTimeDistri(m_engine));
	}
	if (m_canEmit == true)
	{
		m_timerDuration += frameTime;
		if (m_timerDuration > m_timerDurationMax)
		{
			m_canEmit = false;
			m_timerDuration = sf::Time::Zero;
		}
	}
}

void	WaterDropSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	static float const			AngularVelocity = 30.f;

	std::get<Component::Rotation>(particle) += AngularVelocity * frameTime.asSeconds();
	std::get<MyComponent::Time>(particle) += frameTime;
	float ratio = std::get<MyComponent::Time>(particle) / std::get<MyComponent::Life>(particle);
	std::get<Component::Position>(particle) += sf::Vector2f(0.f, 200.f) * ratio * frameTime.asSeconds();
	std::get<Component::Color>(particle).a = 255.f * std::max(0.f, (1.f - std::get<MyComponent::Time>(particle).asSeconds() / std::get<MyComponent::Life>(particle).asSeconds()));
}

bool	WaterDropSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<MyComponent::Time>(particle) >= std::get<MyComponent::Life>(particle));
}
