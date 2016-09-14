#include "ASpecialNpc.hpp"
#include "Progress.hpp"

ASpecialNpc::ASpecialNpc(ResourceKey const & npcId, bool followOcto, bool isMeetable) :
	ANpc(npcId, isMeetable),
	m_canDoSpecial(true),
	m_followOcto(followOcto)
{
	setTextOffset(sf::Vector2f(10.f, 150.f));
	setTimerMax(sf::seconds(8.0f));
	setupBox(this, static_cast<std::size_t>(GameObjectType::SpecialNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void ASpecialNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					specialState;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	specialState = std::make_shared<State>("1", getSpecial1Animation(), getSprite());

	machine.setStart(specialState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, specialState, idleState);

	machine.addTransition(Special1, specialState, specialState);
	machine.addTransition(Special1, idleState, specialState);

	setMachine(machine);
	setNextEvent(Idle);
}

void ASpecialNpc::update(sf::Time frameTime)
{
	if (!getCollideEventOcto())
		m_canDoSpecial = true;
	ANpc::update(frameTime);
}

void ASpecialNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (m_followOcto)
	{
		if (Progress::getInstance().getOctoPos().x < ANpc::getPosition().x)
		{
			getSprite().setOrigin(getSprite().getLocalSize().x - getOrigin().x, getOrigin().y);
			getSprite().setScale(-getScale(), getScale());
		}
		else
		{
			getSprite().setOrigin(getOrigin());
			getSprite().setScale(getScale(), getScale());
		}
	}
	if (sprite.getCurrentEvent() == Idle && getCollideEventOcto() && m_canDoSpecial)
	{
		m_canDoSpecial = false;
		sprite.setNextEvent(Special1);
	}
	else if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
	{
		sprite.setNextEvent(Idle);
	}
}
