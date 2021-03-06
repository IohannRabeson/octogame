#include "ASpecialNpc.hpp"
#include "Progress.hpp"

ASpecialNpc::ASpecialNpc(ResourceKey const & npcId, bool followOcto, bool isMeetable) :
	ANpc(npcId, isMeetable),
	m_generator("random"),
	m_canDoSpecial(true)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::SpecialNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	setFollowOcto(followOcto);
	m_randomSpecial = sf::seconds(m_generator.randomFloat(5.f, 40.f));
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
	m_randomSpecial -= frameTime;
	ANpc::update(frameTime);
}

void ASpecialNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle && ((m_canDoSpecial && getCollideEventOcto()) || m_randomSpecial <= sf::Time::Zero))
	{
		m_canDoSpecial = false;
		m_randomSpecial = sf::seconds(m_generator.randomFloat(5.f, 40.f));
		sprite.setNextEvent(Special1);
	}
	else if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
	{
		sprite.setNextEvent(Idle);
	}
}
