#include "AIdleNpc.hpp"
#include "Progress.hpp"

AIdleNpc::AIdleNpc(ResourceKey const & npcId, bool followOcto, bool isMeetable) :
	ANpc(npcId, isMeetable),
	m_followOcto(followOcto)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::IdleNpc), static_cast<std::size_t>(GameObjectType::Player));
}

void AIdleNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);

	setMachine(machine);
	setNextEvent(Idle);
}

void AIdleNpc::updateState(void)
{
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
}
