#include "SparkSystem.hpp"
#include <Math.hpp>
#include <Interpolations.hpp>
#include <ctime>

SparkSystem::SparkSystem() :
	m_engine(std::time(0)),
	m_creationTimeDistri(0.01f, 0.02f),
	m_lifeTimeDistri(0.2f, 0.3f),
	m_directionDistri(0.f, octo::Pi2),
	m_timer(sf::Time::Zero),
	m_nextCreation(sf::Time::Zero),
	m_color(sf::Color::Red),
	m_canEmit(false)
{
	static float const		Size = 3.f;
	SparkSystem::Prototype	prototype;

	prototype.emplace_back(sf::Vertex({-Size, Size}));
	prototype.emplace_back(sf::Vertex({Size, -Size}));
	prototype.emplace_back(sf::Vertex({-Size, -Size}));
	reset(prototype, sf::Triangles, 2000);
}

void	SparkSystem::canEmit(bool canEmit)
{
	m_canEmit = canEmit;
}

void	SparkSystem::setColor(sf::Color const & color)
{
	m_color = color;
}

void	SparkSystem::setEmitter(sf::Vector2f const & emitter)
{
	m_emitter = emitter;
}

void	SparkSystem::update(sf::Time frameTime)
{
	ParticleSystem::update(frameTime);

	m_timer += frameTime;
	while (m_timer > m_nextCreation)
	{
		if (m_canEmit)
		{
			emplace(m_color, m_emitter, sf::Vector2f(1.f, 1.f), m_directionDistri(m_engine) * 180,
					sf::Time::Zero,
					sf::seconds(m_lifeTimeDistri(m_engine)),
					m_directionDistri(m_engine));
		}
		m_timer -= m_nextCreation;
		m_nextCreation = sf::seconds(m_creationTimeDistri(m_engine));
	}
}

void	SparkSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	static float const			Velocity = 256.f;
	static float const			AngularVelocity = 30.f;
	sf::Vector2f				direction(std::sin(std::get<MyComponent::Direction>(particle)) * Velocity,
									std::cos(std::get<MyComponent::Direction>(particle)) * Velocity);

	std::get<Component::Position>(particle) = std::get<Component::Position>(particle) + direction * frameTime.asSeconds();
	std::get<Component::Rotation>(particle) = std::get<Component::Rotation>(particle) + AngularVelocity * frameTime.asSeconds();
	std::get<MyComponent::Time>(particle) = std::get<MyComponent::Time>(particle) + frameTime;
	float ratio = std::get<MyComponent::Time>(particle) / std::get<MyComponent::Life>(particle);
	std::get<Component::Color>(particle) = octo::linearInterpolation(sf::Color::Yellow, m_color, ratio);
	std::get<Component::Color>(particle).a = 255 * std::max(0.f, (1.f - std::get<MyComponent::Time>(particle).asSeconds() / std::get<MyComponent::Life>(particle).asSeconds()));
}

bool	SparkSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<MyComponent::Time>(particle) >= std::get<MyComponent::Life>(particle));
}
