#include "AWalkNpc.hpp"
#include "RectangleShape.hpp"

AWalkNpc::AWalkNpc(ResourceKey const & npcId, bool isMeetable) :
	ANpc(npcId, isMeetable),
	m_velocity(200.f)
{
	setTimerMax(sf::seconds(0.8f));
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

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	walkLeftState = std::make_shared<State>("1", getWalkAnimation(), getSprite());
	walkRightState = std::make_shared<State>("2", getWalkAnimation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, walkLeftState, idleState);
	machine.addTransition(Idle, walkRightState, idleState);

	machine.addTransition(Left, idleState, walkLeftState);
	machine.addTransition(Left, walkLeftState, walkLeftState);
	machine.addTransition(Left, walkRightState, walkLeftState);

	machine.addTransition(Right, idleState, walkRightState);
	machine.addTransition(Right, walkLeftState, walkRightState);
	machine.addTransition(Right, walkRightState, walkRightState);

	setMachine(machine);
	getSprite().setNextEvent(Idle);
}

void AWalkNpc::updateState(void)
{
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();
	octo::CharacterSprite & sprite = getSprite();

	if (bounds.left <= getArea().left && canWalk() && sprite.getCurrentEvent() == Left)
	{
		sprite.setNextEvent(Right);
		reverseSprite(false);
	}
	else if ((bounds.left + bounds.width) >= (getArea().left + getArea().width) && canWalk() && sprite.getCurrentEvent() == Right)
	{
		sprite.setNextEvent(Left);
		reverseSprite(true);
	}
	else if (sprite.getCurrentEvent() != Idle)
	{
		if (getTimer() >= getTimerMax())
		{
			sprite.setNextEvent(Idle);
			addTimer(-getTimerMax());
		}
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (getTimer() >= getTimerMax())
		{
			sprite.setNextEvent(Left);
			reverseSprite(true);
			addTimer(-getTimerMax());
		}
	}
}

void AWalkNpc::updatePhysics(void)
{
	sf::Vector2f velocity;

	if (getSprite().getCurrentEvent() == Left)
	{
		velocity.x = (-1.f * m_velocity);
	}
	else if (getSprite().getCurrentEvent() == Right)
	{
		velocity.x = m_velocity;
	}
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

