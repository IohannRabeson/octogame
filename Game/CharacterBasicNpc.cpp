#include "CharacterBasicNpc.hpp"
#include "ResourceDefinitions.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <LevelMap.hpp>

CharacterBasicNpc::CharacterBasicNpc()
{
	typedef octo::CharacterAnimation::Frame			Frame;
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;
	octo::ResourceManager&		resources = octo::Application::getResourceManager();
	octo::FiniteStateMachine	machine;
	StatePtr					state0;
	m_rect = sf::FloatRect();
	m_idleAnimation.setFrames({
			Frame(sf::seconds(0.4f), {10, m_rect, sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11, m_rect, sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12, m_rect, sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13, m_rect, sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14, m_rect, sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15, m_rect, sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16, m_rect, sf::Vector2f()})
			});
	m_idleAnimation.setLoop(octo::LoopMode::Loop);
	state0 = std::make_shared<State>("0", m_idleAnimation, m_sprite);

	machine.setStart(state0);
	machine.addTransition(Idle, state0, state0);

	m_sprite.setPosition(sf::Vector2f(-100, 400));
	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_COMPLETE_OSS));
	m_sprite.setMachine(machine);
	m_sprite.restart();
}

void	CharacterBasicNpc::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
}

void	CharacterBasicNpc::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	m_sprite.draw(render, states);
}
