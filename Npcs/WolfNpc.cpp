#include "WolfNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

WolfNpc::WolfNpc(void) :
	ANpc(WOLF_OSS),
	m_engine(std::time(0)),
	m_specialDistribution(1, 2),
	m_lastState(Left)
{
	setSize(sf::Vector2f(190.f, 155.f));
	setOrigin(sf::Vector2f(0.f, 0.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
	setTimerMax(sf::seconds(10.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::WolfNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void WolfNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::NoLoop);

	getWalkAnimation().setFrames({
			Frame(sf::seconds(0.2f), {40u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {41u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {42u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {43u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {44u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {39u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {23u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {24u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(1.4f), {25u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void WolfNpc::setupMachine(void)
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

void WolfNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();
	sf::FloatRect const & area = getArea();
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();

	if (sprite.getCurrentEvent() == Left && bounds.left <= area.left)
	{
		sprite.setNextEvent(Right);
		setWalkEvent(Right);
	}
	else if (sprite.getCurrentEvent() == Right && (bounds.left + bounds.width) >= (area.left + area.width))
	{
		sprite.setNextEvent(Left);
		setWalkEvent(Left);
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(m_lastState);
			setWalkEvent(m_lastState);
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
			if (m_specialDistribution(m_engine) == 1)
				sprite.setNextEvent(Special1);
			else
				sprite.setNextEvent(Idle);
			addTimer(-getTimerMax());
		}
	}
}

void WolfNpc::setWalkEvent(std::size_t event)
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
