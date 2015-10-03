#include "MusicSystem.hpp"
#include <Math.hpp>
#include <Interpolations.hpp>
#include <ctime>

MusicSystem::MusicSystem() :
	m_engine(std::time(0)),
	m_creationTimeDistri(0.4f, 0.6f),
	m_lifeTimeDistri(1.5f, 1.5f),
	m_boolDistri(0.5f),
	m_timer(sf::Time::Zero),
	m_nextCreation(sf::Time::Zero),
	m_color(sf::Color::Black),
	m_canEmit(false)
{
	std::random_device rd;
	m_engine.seed(rd());
	static float const		Size = 4.f;
	MusicSystem::Prototype	prototype;

	prototype.emplace_back(sf::Vertex({2.f * Size, 0.f}));
	prototype.emplace_back(sf::Vertex({3.f * Size, Size}));
	prototype.emplace_back(sf::Vertex({2.f * Size, Size}));

	static float const		Width = 1.f;
	prototype.emplace_back(sf::Vertex({2.f * Size - Width, 0.f}));
	prototype.emplace_back(sf::Vertex({2.f * Size, 0.f}));
	prototype.emplace_back(sf::Vertex({2.f * Size - Width, 3.f * Size}));

	prototype.emplace_back(sf::Vertex({2.f * Size - Width, 3.f * Size}));
	prototype.emplace_back(sf::Vertex({2.f * Size, 3.f * Size}));
	prototype.emplace_back(sf::Vertex({2.f * Size, 0.f}));

	prototype.emplace_back(sf::Vertex({Size, 2.f * Size}));
	prototype.emplace_back(sf::Vertex({2.f * Size, 3.f * Size}));
	prototype.emplace_back(sf::Vertex({Size, 3.f * Size}));

	prototype.emplace_back(sf::Vertex({Size, 2.f * Size}));
	prototype.emplace_back(sf::Vertex({Size, 3.f * Size}));
	prototype.emplace_back(sf::Vertex({0.f, 3.f * Size}));

	prototype.emplace_back(sf::Vertex({Size, 3.f * Size}));
	prototype.emplace_back(sf::Vertex({Size, 4.f * Size}));
	prototype.emplace_back(sf::Vertex({0.f, 3.f * Size}));

	prototype.emplace_back(sf::Vertex({Size, 3.f * Size}));
	prototype.emplace_back(sf::Vertex({2.f * Size, 3.f * Size}));
	prototype.emplace_back(sf::Vertex({Size, 4.f * Size}));
	reset(prototype, sf::Triangles, 2000);
}

void	MusicSystem::canEmit(bool canEmit)
{
	m_canEmit = canEmit;
}

void	MusicSystem::setColor(sf::Color const & color)
{
	m_color = color;
}

void	MusicSystem::setEmitter(sf::Vector2f const & emitter)
{
	m_emitter = emitter;
}

void	MusicSystem::update(sf::Time frameTime)
{
	ParticleSystem::update(frameTime);

	m_timer += frameTime;
	while (m_timer > m_nextCreation)
	{
		if (m_canEmit)
		{
			emplace(m_color, m_emitter, sf::Vector2f(1.f, 1.f), 0,
					sf::Time::Zero,
					sf::seconds(m_lifeTimeDistri(m_engine)),
					m_boolDistri(m_engine));
		}
		m_timer -= m_nextCreation;
		m_nextCreation = sf::seconds(m_creationTimeDistri(m_engine));
	}
}

void	MusicSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	if (std::get<MyComponent::Side>(particle) == true)
		std::get<Component::Position>(particle) += sf::Vector2f(std::sin(std::get<MyComponent::Time>(particle).asSeconds() * octo::Pi2) * 130.f, -100.f) * frameTime.asSeconds();
	else if (std::get<MyComponent::Side>(particle) == false)
		std::get<Component::Position>(particle) += sf::Vector2f(std::cos(std::get<MyComponent::Time>(particle).asSeconds() * octo::Pi2) * 130.f, -100.f) * frameTime.asSeconds();
	std::get<MyComponent::Time>(particle) += frameTime;
	std::get<Component::Color>(particle).a = 255 * std::max(0.f, (1.f - std::get<MyComponent::Time>(particle).asSeconds() / std::get<MyComponent::Life>(particle).asSeconds()));
}

bool	MusicSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<MyComponent::Time>(particle) >= std::get<MyComponent::Life>(particle));
}
