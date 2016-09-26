#include "AIdleNpc.hpp"
#include "Progress.hpp"

AIdleNpc::AIdleNpc(ResourceKey const & npcId, bool followOcto, bool isMeetable) :
	ANpc(npcId, isMeetable)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::IdleNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	setFollowOcto(followOcto);
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
}
