#include "StarSystem.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Math.hpp>
#include <Interpolations.hpp>
#include <ctime>

StarSystem::StarSystem() :
	m_creationTimeDistri(0.01f, 0.02f),
	m_heightDistri(-1000.f, octo::Application::getCamera().getRectangle().height + 1000.f),
	m_speed(-2000.f, 0.f),
	m_timer(sf::Time::Zero),
	m_nextCreation(sf::Time::Zero),
	m_color(sf::Color::White),
	m_canEmit(false)
{
	std::random_device rd;
	m_engine.seed(rd());
}

void StarSystem::setup(sf::Vector2f const & sizeParticle)
{
	m_sizeParticle = sizeParticle;

	StarSystem::Prototype prototype;
	createOctogon(m_sizeParticle, m_sizeParticle / 2.f, prototype);
	reset(prototype, sf::Triangles, 2000);
}

void StarSystem::createTriangle(sf::Vertex const & p1, sf::Vertex const & p2, sf::Vertex const & p3, StarSystem::Prototype & prototype)
{
	prototype.emplace_back(p1);
	prototype.emplace_back(p2);
	prototype.emplace_back(p3);
}

void StarSystem::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, StarSystem::Prototype & prototype)
{
	sf::Vertex origin({0.f, 0.f});
	sf::Vertex upLeft({-size.x + sizeCorner.x, -size.y});
	sf::Vertex upRight({size.x - sizeCorner.x, -size.y});
	sf::Vertex upMidLeft({-size.x, -size.y + sizeCorner.y});
	sf::Vertex upMidRight({size.x, -size.y + sizeCorner.y});
	sf::Vertex downLeft({-size.x + sizeCorner.x, size.y});
	sf::Vertex downRight({size.x - sizeCorner.x, size.y});
	sf::Vertex downMidLeft({-size.x, size.y - sizeCorner.y});
	sf::Vertex downMidRight({size.x, size.y - sizeCorner.y});

	sf::Vertex recDownRight({0.f, -size.y + sizeCorner.y});

	createTriangle(origin, upLeft, upRight, prototype);
	createTriangle(origin, upRight, upMidRight, prototype);
	createTriangle(origin, upMidRight, downMidRight, prototype);
	createTriangle(origin, downMidRight, downRight, prototype);
	createTriangle(origin, downRight, downLeft, prototype);
	createTriangle(origin, downLeft, downMidLeft, prototype);
	createTriangle(origin, downMidLeft, upMidLeft, prototype);
	createTriangle(origin, upMidLeft, upLeft, prototype);
}


void	StarSystem::canEmit(bool canEmit)
{
	m_canEmit = canEmit;
}

void	StarSystem::setEmitTimeRange(float min, float max)
{
	m_creationTimeDistri.param(std::uniform_real_distribution<float>::param_type(min, max));
}

void	StarSystem::setColor(sf::Color const & color)
{
	m_color = color;
}

void	StarSystem::setSpeed(sf::Vector2f speed)
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
