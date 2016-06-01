#include "FatNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

FatNpc::FatNpc(void) :
	ANpc(NPC_FAT_OSS),
	m_engine(std::time(0)),
	m_specialDistribution(1, 10),
	m_lastState(Left)
{
	setSize(sf::Vector2f(25.f, 135.f));
	setOrigin(sf::Vector2f(90.f, 100.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-10.f, -50.f));
	setTimerMax(sf::seconds(2.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void FatNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.6f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getWalkAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void FatNpc::setupMachine(void)
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

	setMachine(machine);
	setNextEvent(Idle);
}

void FatNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();
	sf::FloatRect const & area = getArea();
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();

	if (sprite.getCurrentEvent() == Left && canWalk() && bounds.left <= area.left && canWalk())
	{
		sprite.setNextEvent(Right);
		setWalkEvent(Right);
	}
	else if (sprite.getCurrentEvent() == Right && canWalk() && (bounds.left + bounds.width) >= (area.left + area.width))
	{
		sprite.setNextEvent(Left);
		setWalkEvent(Left);
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (getTimer() >= getTimerMax())
		{
			if (m_specialDistribution(m_engine) == 1)
				sprite.setNextEvent(Special1);
			else
			{
				sprite.setNextEvent(m_lastState);
				setWalkEvent(m_lastState);
			}
			addTimer(-getTimerMax());
		}
	}
	else if (sprite.getCurrentEvent() == Special1)
	{
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(m_lastState);
			setWalkEvent(m_lastState);
		}
	}
	else if (sprite.getCurrentEvent() != Idle)
	{
		if (getTimer() >= getTimerMax())
		{
			m_lastState = sprite.getCurrentEvent();
			sprite.setNextEvent(Idle);
			addTimer(-getTimerMax());
		}
	}
}

void FatNpc::setWalkEvent(std::size_t event)
{
	octo::CharacterSprite & sprite = getSprite();
	if (event == Left)
	{
		sf::Vector2f const & size = sprite.getLocalSize();
		sprite.setOrigin(size.x - getOrigin().x, getOrigin().y);
		sprite.setScale(-getScale(), getScale());
	}
	else if (event == Right)
	{
		sprite.setOrigin(getOrigin().x, getOrigin().y);
		sprite.setScale(getScale(), getScale());
	}
}
