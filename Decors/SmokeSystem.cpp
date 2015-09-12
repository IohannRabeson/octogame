#include "SmokeSystem.hpp"

#include <Application.hpp>
#include <Camera.hpp>
#include <GraphicsManager.hpp>
#include <Math.hpp>
#include <ctime>

SmokeSystem::SmokeSystem() :
	m_emit(false),
	m_engine(std::time(0)),
	m_lifeTimeDistri(0.5f, 1.f),
	m_directionDistri(0.f, 2.f * octo::Pi)
{
	SmokeSystem::Prototype prototype;
	createOctogon(sf::Vector2f(10.f, 10.f), sf::Vector2f(5.f, 5.f), prototype);
	reset(prototype, sf::TrianglesFan, 2000);
	octo::Application::getGraphicsManager().addMouseListener(this);
}

void SmokeSystem::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, SmokeSystem::Prototype & prototype)
{
	sf::Vertex upLeft({-size.x + sizeCorner.x, -size.y});
	sf::Vertex upRight({size.x - sizeCorner.x, -size.y});
	sf::Vertex upMidLeft({-size.x, -size.y + sizeCorner.y});
	sf::Vertex upMidRight({size.x, -size.y + sizeCorner.y});
	sf::Vertex downLeft({-size.x + sizeCorner.x, size.y});
	sf::Vertex downRight({size.x - sizeCorner.x, size.y});
	sf::Vertex downMidLeft({-size.x, size.y - sizeCorner.y});
	sf::Vertex downMidRight({size.x, size.y - sizeCorner.y});

	sf::Vertex recDownRight({0.f, -size.y + sizeCorner.y});

	prototype.emplace_back(upLeft);
	prototype.emplace_back(upRight);
	prototype.emplace_back(upMidRight);
	prototype.emplace_back(downMidRight);
	prototype.emplace_back(downRight);
	prototype.emplace_back(downLeft);
	prototype.emplace_back(downMidLeft);
	prototype.emplace_back(upLeft);
	prototype.emplace_back(upRight);
	prototype.emplace_back(recDownRight);
	prototype.emplace_back(upMidLeft);
}


void	SmokeSystem::onMoved(sf::Event::MouseMoveEvent const& event)
{
	octo::Camera&	camera = octo::Application::getCamera();

	m_emitter = camera.mapPixelToCoords(sf::Vector2i(event.x, event.y));
}

void	SmokeSystem::onPressed(sf::Event::MouseButtonEvent const&)
{
	m_emit = true;
}

void	SmokeSystem::onReleased(sf::Event::MouseButtonEvent const&)
{
	m_emit = false;
}

void	SmokeSystem::update(sf::Time frameTime)
{
	ParticleSystem::update(frameTime);
	if (m_emit)
	{
		emplace(sf::Color::Green, m_emitter, sf::Vector2f(1.f, 1.f), m_directionDistri(m_engine) * 180,
				sf::Time::Zero,
				sf::seconds(m_lifeTimeDistri(m_engine)),
				m_directionDistri(m_engine));
	}
}

void	SmokeSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	static float const			Velocity = 256.f;
	static float const			AngularVelocity = 30.f;
	sf::Vector2f	direction(std::sin(std::get<MyComponent::Direction>(particle)) * Velocity,
							  std::cos(std::get<MyComponent::Direction>(particle)) * Velocity);

	std::get<Component::Position>(particle) = std::get<Component::Position>(particle) + direction * frameTime.asSeconds();
	std::get<Component::Rotation>(particle) = std::get<Component::Rotation>(particle) + AngularVelocity * frameTime.asSeconds();
	std::get<MyComponent::Time>(particle) = std::get<MyComponent::Time>(particle) + frameTime;
	std::get<Component::Color>(particle).a = 255 * std::max(0.f, (1.f - std::get<MyComponent::Time>(particle).asSeconds() / std::get<MyComponent::Life>(particle).asSeconds()));
}

bool	SmokeSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<MyComponent::Time>(particle) >= std::get<MyComponent::Life>(particle));
}

