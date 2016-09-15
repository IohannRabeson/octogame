#include "BeamSystem.hpp"
#include <Math.hpp>
#include <Interpolations.hpp>

BeamSystem::BeamSystem(void) :
	m_cycleTime(sf::seconds(4)),
	m_speedUp(512.f),
	m_width(200.f),
	m_height(300.f),
	m_rotationFactor(0.f),
	m_emitInterval(sf::seconds(0.07f)),
	m_random(std::time(0)),
	m_distri(0.f, 1.f),
	m_biome(nullptr)
{
	float const	size = 16.f;

	reset({sf::Vertex({0.f, 0.f}),
		   sf::Vertex({size, 0.f}),
		   sf::Vertex({size, size})},
		   sf::Triangles, 1000u);
}

void	BeamSystem::setWidth(float width)
{
	m_width = width;
}

void	BeamSystem::setHeight(float height)
{
	m_height = height;
}

void	BeamSystem::setRotationFactor(float factor)
{
	m_rotationFactor = factor;
}

void	BeamSystem::setBiome(ABiome & biome)
{
	m_biome = &biome;
}

float	BeamSystem::getHeight(void) const
{
	return m_height;
}

float	BeamSystem::getWidth(void) const
{
	return m_width;
}

void	BeamSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	sf::Vector2f	position = std::get<Component::Position>(particle);
	sf::Time		currentTime = std::get<MyComponent::ElapsedTime>(particle) + frameTime;
	sf::Color&		color = std::get<Component::Color>(particle);
	float			cycle = currentTime / m_cycleTime;
	float			heightPos = 0.f;

	position.y -= frameTime.asSeconds() * std::get<MyComponent::UpSpeed>(particle);
	heightPos = (position.y / m_height);
	position.x = std::cos(cycle * octo::Pi2) * m_width * 0.5f;
	color.a = heightPos * 255;
	if (currentTime >= m_cycleTime)
	{
		currentTime -= m_cycleTime;
	}
	std::get<MyComponent::ElapsedTime>(particle) = currentTime;
	std::get<Component::Position>(particle) = position;
	std::get<Component::Rotation>(particle) += m_rotationFactor * std::get<MyComponent::AngularVelocity>(particle);
	std::get<MyComponent::UpSpeed>(particle) *= 1.0001f;
}

void	BeamSystem::update(sf::Time frameTime)
{
	bool	emitParticle = false;

	m_emitTimer += frameTime;
	while (m_emitTimer > m_emitInterval)
	{
		emitParticle = true;
		m_emitTimer -= m_emitInterval;
	}
	if (emitParticle)
		createParticle();
	ParticleSystem::update(frameTime);
}

void	BeamSystem::createParticle()
{
	sf::Color color;
	if (m_biome)
		color = m_biome->getParticleColorGround();
	else
		color = sf::Color::White;
	emplace(color,
			sf::Vector2f(0, 0),
			sf::Vector2f(1.f, 1.f),
			m_distri(m_random) * 360.f,
			sf::seconds(m_distri(m_random) * m_cycleTime.asSeconds()),
			std::max(0.2f, m_distri(m_random)) * m_speedUp,
			m_distri(m_random) * 720 - 360
			);
}

bool	BeamSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<Component::Position>(particle).y < -m_height);
}

