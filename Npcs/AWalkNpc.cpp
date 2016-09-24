#include "AWalkNpc.hpp"
#include "RectangleShape.hpp"

AWalkNpc::AWalkNpc(ResourceKey const & npcId, bool isMeetable) :
	ANpc(npcId, isMeetable),
	m_generator("random"),
	m_lastState(Left),
	m_velocity(50.f)
{
	setTimerMax(sf::seconds(10.f));
	setupBox(this, static_cast<std::size_t>(GameObjectType::WalkNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void AWalkNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					walkLeftState;
	StatePtr					walkRightState;
	StatePtr					special1State;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	walkLeftState = std::make_shared<State>("1", getWalkAnimation(), getSprite());
	walkRightState = std::make_shared<State>("2", getWalkAnimation(), getSprite());
	special1State = std::make_shared<State>("3", getSpecial1Animation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, walkLeftState, idleState);
	machine.addTransition(Idle, walkRightState, idleState);

	machine.addTransition(Left, idleState, walkLeftState);
	machine.addTransition(Left, walkLeftState, walkLeftState);
	machine.addTransition(Left, walkRightState, walkLeftState);
	machine.addTransition(Left, special1State, walkLeftState);

	machine.addTransition(Right, idleState, walkRightState);
	machine.addTransition(Right, walkLeftState, walkRightState);
	machine.addTransition(Right, walkRightState, walkRightState);
	machine.addTransition(Right, special1State, walkRightState);

	machine.addTransition(Special1, idleState, special1State);
	machine.addTransition(Special1, walkLeftState, special1State);
	machine.addTransition(Special1, walkRightState, special1State);

	setMachine(machine);
	setNextEvent(Idle);
}

void AWalkNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();
	sf::FloatRect const & area = getArea();
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();

	if (sprite.getCurrentEvent() == Left && bounds.left <= area.left)
	{
		reverseSprite(false);
		sprite.setNextEvent(Right);
	}
	else if (sprite.getCurrentEvent() == Right && (bounds.left + bounds.width) >= (area.left + area.width))
	{
		reverseSprite(true);
		sprite.setNextEvent(Left);
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(m_lastState);
			if (m_lastState == Left)
				reverseSprite(true);
			else if (m_lastState == Right)
				reverseSprite(false);
		}
	}
	else if (sprite.getCurrentEvent() == Special1)
	{
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(m_lastState);
			if (m_lastState == Left)
				reverseSprite(true);
			else if (m_lastState == Right)
				reverseSprite(false);
		}
	}
	else if (sprite.getCurrentEvent() != Idle)
	{
		if (getTimer() >= getTimerMax())
		{
			m_lastState = sprite.getCurrentEvent();
			if (m_generator.randomBool(0.5f))
				sprite.setNextEvent(Special1);
			else
				sprite.setNextEvent(Idle);
			addTimer(-getTimerMax());
		}
	}
}

void AWalkNpc::updatePhysics(void)
{
	sf::Vector2f velocity;

	if (getSprite().getCurrentEvent() == Left)
		velocity.x = (-1.f * m_velocity);
	else if (getSprite().getCurrentEvent() == Right)
		velocity.x = m_velocity;

	getBox()->setVelocity(velocity);
}

float AWalkNpc::getVelocity(void) const
{
	return m_velocity;
}

void AWalkNpc::setVelocity(float velocity)
{
	m_velocity = velocity;
}

