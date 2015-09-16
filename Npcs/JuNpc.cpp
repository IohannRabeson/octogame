#include "JuNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

JuNpc::JuNpc(void) :
	ANpc(JU_OSS)
{
	setSize(sf::Vector2f(35.f, 75.f));
	setOrigin(sf::Vector2f(75.f, 68.f));
	setScale(0.6f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::JuNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void JuNpc::setup(void)
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
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()})
			});
	getIdleAnimation().setLoop(octo::LoopMode::NoLoop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17u, sf::FloatRect(), sf::Vector2f()})
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void JuNpc::setupMachine(void)
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

void JuNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	m_timer += frametime;
	if (m_timer > sf::seconds(10.f))
	{
		m_canSpecial = true;
		m_timer = sf::Time::Zero;
	}
	else
		m_canSpecial = false;

	updateState();
	updatePhysics();

	sprite.update(frametime);
	sf::Vector2f const & center = getBox()->getRenderPosition();
	sprite.setPosition(center);

	updateText(frametime);
	resetVariables();
}

bool JuNpc::canSpecial1(void) const
{
	if (m_canSpecial == true)
		return true;
	return false;
}

void JuNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle && sprite.isTerminated())
		sprite.setNextEvent(Special1);
	else if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
		sprite.setNextEvent(Idle);
}

void JuNpc::updatePhysics(void)
{
}

void JuNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
}

