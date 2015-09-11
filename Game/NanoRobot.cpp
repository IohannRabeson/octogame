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
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_swarm.setPositionBehavior(new FireflySwarm::CirclePositionBehavior(2345, 50.f));
	m_swarm.setTexture(resources.getTexture(FIREFLY01_PNG));
	m_swarm.create(m_spawnMode, position, sf::Color::Magenta, 8.f, 32.f, 2.f);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(NANO1_OSS));
	m_sprite.setScale(0.6f, 0.6f);

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.2f), 0);
	frames.emplace_back(sf::seconds(0.2f), 1);
	frames.emplace_back(sf::seconds(0.2f), 2);
	frames.emplace_back(sf::seconds(0.2f), 3);

	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::Loop);
	m_sprite.setAnimation(m_animation);
	m_sprite.play();
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
	m_sprite.update(frametime);
	m_sprite.setPosition(m_swarm.getFirefly(0u).position - sf::Vector2f(32.f, 32.f));
}

void NanoRobot::draw(sf::RenderTarget& render) const
{
	//m_swarm.draw(render);
	render.draw(m_sprite);
}
