#include "CheckPoint.hpp"
#include "CharacterOcto.hpp"
#include "Progress.hpp"

CheckPoint::CheckPoint(void) :
	ANpc(CHECKPOINT_OSS),
	m_firstFrame(true)
{
	setSize(sf::Vector2f(30.f, 85.f));
	setOrigin(sf::Vector2f(27.f, 275.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-20.f, -10.f));
	setTimerMax(sf::seconds(8.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void CheckPoint::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void CheckPoint::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					special1State;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1State = std::make_shared<State>("1", getSpecial1Animation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, special1State, idleState);

	machine.addTransition(Special1, idleState, special1State);
	machine.addTransition(Special1, special1State, special1State);

	setMachine(machine);
	setNextEvent(Idle);
}

void CheckPoint::updateState(void)
{
	if (m_firstFrame)
	{
		m_firstFrame = false;
		m_startPosition = getPosition();
	}
}

void CheckPoint::collideOctoEvent(CharacterOcto *)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Idle)
		sprite.setNextEvent(Special1);
	Progress::getInstance().setCheckPointPosition(m_startPosition);
}
