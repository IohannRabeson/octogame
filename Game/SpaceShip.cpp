#include "SpaceShip.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include "ResourceDefinitions.hpp"

SpaceShip::SpaceShip(SpaceShipEvents event)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(SPACESHIP_OSS));
	m_sprite.setScale(0.6f, 0.6f);

	typedef octo::CharacterAnimation::Frame		Frame;
	std::vector<Frame>							frames;
	frames.push_back(Frame(sf::seconds(0.2f), { 0, sf::FloatRect(), sf::Vector2f() }));
	m_flyingAnimation.setFrames(frames);
	m_flyingAnimation.setLoop(octo::LoopMode::Loop);

	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					flyingState;
	StatePtr					brokenState;

	flyingState = std::make_shared<State>("0", m_flyingAnimation, m_sprite);
	brokenState = std::make_shared<State>("1", m_brokenAnimation, m_sprite);

	machine.setStart(flyingState);
	machine.addTransition(Flying, flyingState, flyingState);
	machine.addTransition(Flying, brokenState, flyingState);

	machine.addTransition(Broken, brokenState, brokenState);
	machine.addTransition(Broken, flyingState, brokenState);

	m_sprite.setMachine(machine);
	m_sprite.setNextEvent(event);
}

void SpaceShip::setNextEvent(SpaceShipEvents event)
{
	m_sprite.setNextEvent(event);
}

sf::Vector2f SpaceShip::getSize(void) const
{
	return m_sprite.getLocalSize();
}

sf::Vector2f const & SpaceShip::getPosition(void) const
{
	return m_sprite.getPosition();
}

void SpaceShip::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
}

void SpaceShip::draw(sf::RenderTarget& render) const
{
	render.draw(m_sprite);
}
