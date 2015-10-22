#include "ClementineNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

ClementineNpc::ClementineNpc(void) :
	ANpc(CLEMENTINE_OSS)
{
	setSize(sf::Vector2f(50.f, 75.f));
	setOrigin(sf::Vector2f(90.f, 100.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -70.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::ClementineNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void ClementineNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.6f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void ClementineNpc::setupMachine(void)
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

void ClementineNpc::update(sf::Time frametime)
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

bool ClementineNpc::canSpecial1(void) const
{
	if (m_canSpecial == true)
		return true;
	return false;
}

void ClementineNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();
	sf::Vector2f const & size = sprite.getLocalSize();
	sprite.setOrigin(size.x - getOrigin().x, getOrigin().y);
	sprite.setScale(-getScale(), getScale());

	if (sprite.getCurrentEvent() == Idle && m_canSpecial)
		sprite.setNextEvent(Special1);
	else if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
		sprite.setNextEvent(Idle);
}

void ClementineNpc::updatePhysics(void)
{
}

void ClementineNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
}

