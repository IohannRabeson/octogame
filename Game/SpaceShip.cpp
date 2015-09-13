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
	frames.push_back(Frame(sf::seconds(0.2f), { 1, sf::FloatRect(), sf::Vector2f() }));
	frames.push_back(Frame(sf::seconds(0.2f), { 2, sf::FloatRect(), sf::Vector2f() }));
	frames.push_back(Frame(sf::seconds(0.2f), { 3, sf::FloatRect(), sf::Vector2f() }));
	m_brokenAnimation.setFrames(frames);
	m_brokenAnimation.setLoop(octo::LoopMode::Loop);

	frames.clear();
	frames.push_back(Frame(sf::seconds(0.2f), { 0, sf::FloatRect(), sf::Vector2f() }));
	frames.push_back(Frame(sf::seconds(0.2f), { 3, sf::FloatRect(), sf::Vector2f() }));
	m_flyingAnimation.setFrames(frames);
	m_flyingAnimation.setLoop(octo::LoopMode::Loop);

	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					flyingState;
	StatePtr					brokenState;

	flyingState = std::make_shared<State>("0", m_flyingAnimation, m_sprite);
	brokenState = std::make_shared<State>("1", m_brokenAnimation, m_sprite);

	machine.setStart(brokenState);
	machine.addTransition(Flying, flyingState, flyingState);
	machine.addTransition(Flying, brokenState, flyingState);

	machine.addTransition(Broken, brokenState, brokenState);
	machine.addTransition(Broken, flyingState, brokenState);

	m_sprite.setMachine(machine);
	m_sprite.restart();
	m_sprite.setNextEvent(event);

}

void SpaceShip::setNextEvent(SpaceShipEvents event)
{
	m_sprite.setNextEvent(event);
}

void SpaceShip::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position.x, position.y - m_sprite.getLocalSize().y * 0.467f);
	m_smoke.setPosition(position + sf::Vector2f(140.f, -110.f));
}

void SpaceShip::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
	m_smoke.update(frameTime);
}

void SpaceShip::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	render.draw(m_sprite);
	m_smoke.draw(render);
}
