#include "SmokeSystem.hpp"

#include <Application.hpp>
#include <Camera.hpp>
#include <GraphicsManager.hpp>
#include <Math.hpp>
#include <ctime>

SmokeSystem::SmokeSystem() :
	m_size(10.f, 10.f),
	m_color(255, 255, 255, 150),
	m_velocity(0.f, -256.f),
	m_lifeScaleFactor(15.f),
	m_engine(std::time(0)),
	m_emitIntervalDistri(0.01f, 0.2f),
	m_growTimeDistri(1.5f, 2.5f),
	m_lifeTimeDistri(2.5f, 4.5f),
	m_sideDistri(0, 3),
	m_scaleDistri(1.f, 2.5f)
{
	SmokeSystem::Prototype prototype;
	createOctogon(m_size, m_size / 2.f, prototype);
	reset(prototype, sf::Triangles, 2000);
}

void SmokeSystem::createTriangle(sf::Vertex const & p1, sf::Vertex const & p2, sf::Vertex const & p3, SmokeSystem::Prototype & prototype)
{
	prototype.emplace_back(p1);
	prototype.emplace_back(p2);
	prototype.emplace_back(p3);
}

void SmokeSystem::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, SmokeSystem::Prototype & prototype)
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

	createTriangle(upLeft, upRight, recDownRight, prototype);
	createTriangle(recDownRight, upMidLeft, upLeft, prototype);
}

void	SmokeSystem::createParticle(void)
{
	float scale = m_scaleDistri(m_engine);

	emplace(m_color, m_emitter, sf::Vector2f(scale, scale), 0.f,
			sf::Time::Zero,
			sf::seconds(m_growTimeDistri(m_engine)),
			sf::Time::Zero,
			sf::seconds(m_lifeTimeDistri(m_engine)),
			m_sideDistri(m_engine));
}

void	SmokeSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	sf::Vector2f &		position = std::get<Component::Position>(particle);
	sf::Color &			color = std::get<Component::Color>(particle);
	sf::Vector2f &		scale = std::get<Component::Scale>(particle);

	sf::Time &			lifeTimer = std::get<MyComponent::LifeTimer>(particle);
	sf::Time const &	lifeTimerMax = std::get<MyComponent::LifeTimerMax>(particle);
	sf::Time &			growTimer = std::get<MyComponent::GrowTimer>(particle);
	sf::Time const &	growTimerMax = std::get<MyComponent::GrowTimerMax>(particle);
	int const &			behaviour = std::get<MyComponent::Behaviour>(particle);

	float const &		frameTimeSeconds = frameTime.asSeconds();

	float scaleFactor = 1.f;
	float velocityFactor = 1.f;
	if (growTimer < growTimerMax)
	{
		growTimer += frameTime;
		float growCycle = growTimer / growTimerMax;
		color.a = m_color.a * (growCycle);
		switch (behaviour)
		{
			case 0:
				position.x += std::sin(growCycle * octo::Pi2) * frameTimeSeconds * (30.f * growCycle);
				break;
			case 1:
				position.x -= std::sin(growCycle * octo::Pi2) * frameTimeSeconds * (30.f * growCycle);
				break;
			case 2:
				position.x += std::cos(growCycle * octo::Pi2) * frameTimeSeconds * (30.f * growCycle);
				break;
			case 3:
				position.x -= std::cos(growCycle * octo::Pi2) * frameTimeSeconds * (30.f * growCycle);
				break;
			default:
				break;
		}
	}
	else
	{
		lifeTimer += frameTime;
		float lifeCycle = lifeTimer / lifeTimerMax;
		color.a = m_color.a * std::max(0.f, (1.f - lifeCycle));
		scaleFactor = 1.f * (1.f + (m_lifeScaleFactor * lifeCycle));
		velocityFactor = 1.f - lifeCycle;
		switch (behaviour)
		{
			case 0:
				position.x += std::sin(lifeCycle * octo::Pi) * frameTimeSeconds * (lifeCycle * 300.f);
				break;
			case 1:
				position.x -= std::sin(lifeCycle * octo::Pi) * frameTimeSeconds * (lifeCycle * 300.f);
				break;
			case 2:
				position.x += std::cos(lifeCycle * octo::Pi) * frameTimeSeconds * (lifeCycle * 300.f);
				break;
			case 3:
				position.x -= std::cos(lifeCycle * octo::Pi) * frameTimeSeconds * (lifeCycle * 300.f);
				break;
			default:
				break;
		}
	}
	position = position + m_velocity * velocityFactor * frameTimeSeconds;
	scale = scale + sf::Vector2f(frameTimeSeconds * scaleFactor, frameTimeSeconds * scaleFactor);
}

bool	SmokeSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<MyComponent::LifeTimer>(particle) >= std::get<MyComponent::LifeTimerMax>(particle));
}

void	SmokeSystem::update(sf::Time frameTime)
{
	bool	emitParticle = false;

	m_emitTimer += frameTime;
	while (m_emitTimer > m_emitInterval)
	{
		emitParticle = true;
		m_emitTimer -= m_emitInterval;
		m_emitInterval = sf::seconds(m_emitIntervalDistri(m_engine));
	}
	if (emitParticle)
		createParticle();
	ParticleSystem::update(frameTime);
}

void	SmokeSystem::setPosition(sf::Vector2f const & position)
{
	m_emitter = position;
}

void	SmokeSystem::setVelocity(sf::Vector2f const & velocity)
{
	m_velocity = velocity;
}

void	SmokeSystem::setEmitTimeMax(float min, float max)
{
	m_emitIntervalDistri.param(std::uniform_real_distribution<float>::param_type(min, max));
}

void	SmokeSystem::setColor(sf::Color const & color)
{
	m_color = color;
}
