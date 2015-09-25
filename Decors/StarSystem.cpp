#include "StarSystem.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Math.hpp>
#include <Interpolations.hpp>
#include <ctime>

StarSystem::StarSystem() :
	m_engine(std::time(0)),
	m_creationTimeDistri(0.05f, 0.1f),
	m_heightDistri(-200.f, octo::Application::getCamera().getRectangle().height + 200.f),
	m_speed(-1500.f, 0.f),
	m_timer(sf::Time::Zero),
	m_nextCreation(sf::Time::Zero),
	m_color(sf::Color::White),
	m_canEmit(false)
{
	static float const		Size = 4.f;
	StarSystem::Prototype	prototype;

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

void	StarSystem::canEmit(bool canEmit)
{
	m_canEmit = canEmit;
}

void	StarSystem::setColor(sf::Color const & color)
{
	m_color = color;
}

void	StarSystem::setSpeed(sf::Vector2f const & speed)
{
	m_speed = speed;
}

void	StarSystem::update(sf::Time frameTime)
{
	ParticleSystem::update(frameTime);

	m_timer += frameTime;
	while (m_timer > m_nextCreation)
	{
		if (m_canEmit)
		{
			sf::Vector2f position = sf::Vector2f(20.f + octo::Application::getCamera().getRectangle().left + octo::Application::getCamera().getRectangle().width, octo::Application::getCamera().getRectangle().top);
			position += sf::Vector2f(0.f, m_heightDistri(m_engine));
			emplace(m_color, position, sf::Vector2f(1.f, 1.f), 0, sf::Time::Zero);
		}
		m_timer -= m_nextCreation;
		m_nextCreation = sf::seconds(m_creationTimeDistri(m_engine));
	}
}

void	StarSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	std::get<Component::Position>(particle) += m_speed * frameTime.asSeconds();
}

bool	StarSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<Component::Position>(particle).x < octo::Application::getCamera().getRectangle().left - 20.f);
}
