#include "RocketEnd.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

RocketEnd::RocketEnd(sf::Vector2f const & position) :
	m_smokesCount(3),
	m_smokes(new SmokeSystem[m_smokesCount])
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	m_spriteRocket.setSpriteSheet(resources.getSpriteSheet(ROCKET_OSS));
	m_spriteRocket.setScale(sf::Vector2f(0.7f, 0.7f));
	m_spriteOcto.setSpriteSheet(resources.getSpriteSheet(OCTO_OSS));
	m_spriteOcto.setScale(sf::Vector2f(-0.4f, 0.4f));

	typedef octo::CharacterAnimation::Frame			Frame;

	m_animationRocket.setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			});
	m_animationRocket.setLoop(octo::LoopMode::NoLoop);

	m_animationOcto.setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			});
	m_animationOcto.setLoop(octo::LoopMode::NoLoop);

	setupMachine();

	for (std::size_t i = 0u; i < m_smokesCount; i++)
	{
		m_smokes[i].setVelocity(sf::Vector2f(0.f, 300.f));
		m_smokes[i].setup(sf::Vector2f(10.f, 10.f));
		m_smokes[i].setLifeTimeRange(0.2f, 0.4f);
		m_smokes[i].setEmitTimeRange(0.01f, 0.05f);
	}

	m_position = position;
}

void RocketEnd::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;

	idleState = std::make_shared<State>("0", m_animationRocket, m_spriteRocket);

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);

	m_spriteRocket.setMachine(machine);
	m_spriteRocket.restart();
	m_spriteRocket.setNextEvent(Idle);

	octo::FiniteStateMachine	machineOcto;
	StatePtr					idleStateOcto;

	idleStateOcto = std::make_shared<State>("0", m_animationOcto, m_spriteOcto);

	machineOcto.setStart(idleStateOcto);
	machineOcto.addTransition(Idle, idleStateOcto, idleStateOcto);

	m_spriteOcto.setMachine(machineOcto);
	m_spriteOcto.restart();
	m_spriteOcto.setNextEvent(Idle);
}

void RocketEnd::setPosition(sf::Vector2f const & position)
{
	m_spriteRocket.setPosition(position);
	m_spriteOcto.setPosition(position + sf::Vector2f(125.f, 375.f));
	m_smokes[0].setPosition(position + sf::Vector2f(35.f, 970.f));
	m_smokes[1].setPosition(position + sf::Vector2f(110.f, 970.f));
	m_smokes[2].setPosition(position + sf::Vector2f(185.f, 970.f));
}

void RocketEnd::update(sf::Time frameTime)
{
	m_offset += sf::Vector2f(frameTime.asSeconds() * 2.f, frameTime.asSeconds() * 3.f);
	setPosition(m_position + sf::Vector2f(std::cos(m_offset.x) * 10.f, std::sin(m_offset.y) * 5.f));
	m_spriteRocket.update(frameTime);
	m_spriteOcto.update(frameTime);
	for (std::size_t i = 0u; i < m_smokesCount; i++)
		m_smokes[i].update(frameTime);
}

void RocketEnd::drawFront(sf::RenderTarget & render, sf::RenderStates states) const
{
	m_spriteOcto.draw(render, states);
	m_spriteRocket.draw(render, states);
	for (std::size_t i = 0u; i < m_smokesCount; i++)
		m_smokes[i].draw(render);
}
