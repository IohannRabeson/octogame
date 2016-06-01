#include "JeffMouffyNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include <Interpolations.hpp>

JeffMouffyNpc::JeffMouffyNpc(void) :
	ANpc(JEFF_MOUFFY_OSS),
	m_startTimer(false),
	m_animationEnd(false)
{
	setSize(sf::Vector2f(1.f, 75.f));
	setOrigin(sf::Vector2f(90.f, 193.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(100.f, -80.f));
	setTimerMax(sf::seconds(5.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void JeffMouffyNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.2f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {23u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void JeffMouffyNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					special1;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1 = std::make_shared<State>("3", getSpecial1Animation(), getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, special1, idle);

	machine.addTransition(Special1, special1, special1);
	machine.addTransition(Special1, idle, special1);

	setMachine(machine);
	setNextEvent(Idle);
}

void JeffMouffyNpc::setPosition(sf::Vector2f const & position)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1)
		return;
	ANpc::setPosition(position);
}

void JeffMouffyNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle)
	{
		if (!m_startTimer && getCollideEventOcto())
		{
			m_startTimer = true;
			setTimer(sf::Time::Zero);
		}
		if (m_startTimer)
		{
			if (getTimer() > getTimerMax())
			{
				setDisplayText(false);
				sprite.setNextEvent(Special1);
				m_startPosition = getPosition();
				setTimer(sf::Time::Zero);
			}
		}
	}
}

void JeffMouffyNpc::updatePhysics(void)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1)
	{
		if (!sprite.isTerminated())
			setTimer(sf::Time::Zero);
		else if (sprite.isTerminated())
		{
			if (m_animationEnd == false)
			{
				RectangleShape * box = getBox();
				box->setPosition(octo::linearInterpolation(m_startPosition, m_startPosition + sf::Vector2f(0.f, -2000.f), getTimer() / getTimerMax()));
				box->update();
			}
			if (getTimer() > getTimerMax())
			{
				setCurrentText(1u);
				setTextOffset(sf::Vector2f(0.f, -120.f));
				setDisplayText(true);
				m_animationEnd = true;
			}
		}
	}
}

void JeffMouffyNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
}

void JeffMouffyNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
//	if (!m_animationEnd)
		ANpc::draw(render, states);
}
