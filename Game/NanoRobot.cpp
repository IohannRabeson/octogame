#include "NanoRobot.hpp"
#include "FireflyPositionBehaviors.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include "ResourceDefinitions.hpp"

NanoRobot::NanoRobot(sf::Vector2f const & position) :
	m_swarm(1),
	m_uniformPopulation(1234u, &octo::Application::getResourceManager().getPalette(FROM_SEA1_OPA),
						1.2f, 2.f, 6.f, 10.f, 32.f, 50.f,
						sf::Time::Zero, sf::Time::Zero),
	m_spawnMode(FireflySwarm::SpawnMode::Normal)
{
	m_swarm.setPositionBehavior(new FireflySwarm::CirclePositionBehavior(2345, 50.f));
	m_swarm.setTexture(octo::Application::getResourceManager().getTexture(FIREFLY01_PNG));

	m_swarm.create(m_spawnMode, position, sf::Color::Magenta, 8.f, 32.f, 1.f);
}

NanoRobot::~NanoRobot(void)
{}

void NanoRobot::setPosition(sf::Vector2f const & position)
{
	m_swarm.setTarget(position);
}

void NanoRobot::update(sf::Time frametime)
{
	m_swarm.update(frametime);
}

void NanoRobot::draw(sf::RenderTarget& render) const
{
	m_swarm.draw(render);
}
