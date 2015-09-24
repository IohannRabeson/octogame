#include "IohannNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

IohannNpc::IohannNpc(void) :
	ANpc(IOHANN_OSS),
	m_engine(std::time(0)),
	m_specialDistribution(1, 10),
	m_lastState(Left)
{
	setSize(sf::Vector2f(25.f, 75.f));
	setOrigin(sf::Vector2f(90.f, 100.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
	setTimerMax(sf::seconds(15.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::IohannNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void IohannNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

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
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void IohannNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					walkLeftState;
	StatePtr					walkRightState;
	StatePtr					special1State;

	walkLeftState = std::make_shared<State>("0", getWalkAnimation(), getSprite());
	walkRightState = std::make_shared<State>("1", getWalkAnimation(), getSprite());
	special1State = std::make_shared<State>("2", getSpecial1Animation(), getSprite());

	machine.setStart(walkLeftState);
	machine.addTransition(Left, walkLeftState, walkLeftState);
	machine.addTransition(Left, walkRightState, walkLeftState);
	machine.addTransition(Left, special1State, walkLeftState);

	machine.addTransition(Right, walkLeftState, walkRightState);
	machine.addTransition(Right, walkRightState, walkRightState);
	machine.addTransition(Right, special1State, walkRightState);

	machine.addTransition(Special1, walkLeftState, special1State);
	machine.addTransition(Special1, walkRightState, special1State);

	setMachine(machine);
	setNextEvent(Left);
}

void IohannNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();
	sf::FloatRect const & area = getArea();
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();

	if (sprite.getCurrentEvent() == Left && canWalk() && bounds.left <= area.left)
	{
		sprite.setNextEvent(Right);
		setWalkEvent(Right);
	}
	else if (sprite.getCurrentEvent() == Right && canWalk() && (bounds.left + bounds.width) >= (area.left + area.width))
	{
		sprite.setNextEvent(Left);
		setWalkEvent(Left);
	}
	else if (sprite.getCurrentEvent() == Special1)
	{
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(m_lastState);
			setWalkEvent(m_lastState);
		}
	}
	else
	{
		if (getTimer() >= getTimerMax())
		{
			m_lastState = sprite.getCurrentEvent();
			sprite.setNextEvent(Special1);
			addTimer(-getTimerMax());
		}
	}
}

void IohannNpc::setWalkEvent(std::size_t event)
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
