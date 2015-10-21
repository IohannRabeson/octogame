#include "JellyfishNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include <Interpolations.hpp>

JellyfishNpc::JellyfishNpc(void) :
	ANpc(JELLYFISH_OSS),
	m_startTimer(false),
	m_animationEnd(false)
{
	setSize(sf::Vector2f(10.f, 275.f));
	setOrigin(sf::Vector2f(90.f, 13.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(100.f, -80.f));
	setTimerMax(sf::seconds(100.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::LucienNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void JellyfishNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(1.f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void JellyfishNpc::setupMachine(void)
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

void JellyfishNpc::setPosition(sf::Vector2f const & position)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1)
		return;
	ANpc::setPosition(position);
}

void JellyfishNpc::updateState(void)
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
			sprite.setNextEvent(Special1);
			m_startPosition = getPosition();
			setTimer(sf::Time::Zero);
		}
	}
}

void JellyfishNpc::updatePhysics(void)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1)
	{
		RectangleShape * box = getBox();
		box->setPosition(octo::linearInterpolation(m_startPosition, m_startPosition + sf::Vector2f(0.f, -10000.f), getTimer() / getTimerMax() * 5.f));
		box->update();
		if (getTimer() > getTimerMax() * 5.f)
			m_animationEnd = true;
	}
}

void JellyfishNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
}

void JellyfishNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (!m_animationEnd)
		ANpc::draw(render, states);
}
