#include "CharacterNpc.hpp"
#include "ResourceDefinitions.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <LevelMap.hpp>

CharacterNpc::CharacterNpc()
{
	typedef octo::CharacterAnimation::Frame			Frame;
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;
	octo::ResourceManager&		resources = octo::Application::getResourceManager();
	octo::FiniteStateMachine	machine;
	StatePtr					state0;
	StatePtr					state1;
	StatePtr					state2;
	StatePtr					state3;

	m_idleAnimation.setFrames({
			Frame(sf::seconds(0.4f), {10, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16, sf::FloatRect(), sf::Vector2f()})
			});
	m_idleAnimation.setLoop(octo::LoopMode::Loop);

	m_walkAnimation.setFrames({
			Frame(sf::seconds(0.4f), {0, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6, sf::FloatRect(), sf::Vector2f()})
			});
	m_walkAnimation.setLoop(octo::LoopMode::Loop);

	m_jumpAnimation.setFrames({
			Frame(sf::seconds(0.4f), {16, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {18, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {19, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {20, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {21, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {22, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {23, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {24, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {25, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {26, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {27, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {28, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {29, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {30, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {31, sf::FloatRect(), sf::Vector2f()})
			});
	m_jumpAnimation.setLoop(octo::LoopMode::Loop);

	state0 = std::make_shared<State>("0", m_idleAnimation, m_sprite);
	state1 = std::make_shared<State>("1", m_jumpAnimation, m_sprite);
	state2 = std::make_shared<State>("2", m_walkAnimation, m_sprite);

	machine.setStart(state0);
	machine.addTransition(Left, state0, state1);
	machine.addTransition(Left, state2, state1);
	machine.addTransition(Right, state0, state2);
	machine.addTransition(Right, state1, state2);
	machine.addTransition(Idle, state0, state0);
	machine.addTransition(Idle, state1, state0);
	machine.addTransition(Idle, state2, state0);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_COMPLETE_OSS));
	m_sprite.setMachine(machine);
	m_sprite.restart();
	m_idle = false;
	m_velocity = 0.5f;
}

void	CharacterNpc::update(sf::Time frameTime)
{
	if (m_sprite.getPosition().x == m_area.left && canWalk()){
		m_sprite.setNextEvent(Right);
		m_sprite.setScale(1, 1);
		if (m_velocity == -0.5f)
			m_sprite.setOrigin(m_sprite.getOrigin().x - 177, 0);
		m_velocity = 0.5f;
	}
	else if (m_sprite.getPosition().x == (m_area.left + m_area.width) && canWalk()){
		m_sprite.setNextEvent(Left);
		m_sprite.setScale(-1, 1);
		m_sprite.setOrigin(m_sprite.getOrigin().x + 177, 0);
		m_velocity = -0.5f;
	}
	if (canWalk())
		m_sprite.setPosition(m_sprite.getPosition() + sf::Vector2f(m_velocity, 0.f));
	if ((m_sprite.getPosition().x == 0.f || m_sprite.getPosition().x == (m_area.left + m_area.width))
			&& !m_idle){
		m_sprite.setNextEvent(Idle);
		m_clock.restart();
		m_idle = true;
	}
	m_sprite.update(frameTime);
}

void	CharacterNpc::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	m_sprite.draw(render, states);
}

bool	CharacterNpc::canWalk()
{
	if (!m_idle)
		return true;
	if (m_clock.getElapsedTime().asSeconds() > 2.4f){
		m_idle = false;
		return true;
	}
	return false;
}
