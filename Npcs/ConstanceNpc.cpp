#include "ConstanceNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

ConstanceNpc::ConstanceNpc(void) :
	ANpc(CONSTANCE_OSS)
{
	setSize(sf::Vector2f(1.f, 145.f));
	setOrigin(sf::Vector2f(90.f, 68.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
	setup();

	m_puffTimerMax = sf::seconds(0.8f);
	setupBox(this, static_cast<std::size_t>(GameObjectType::ConstanceNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void ConstanceNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();

	m_smoke.setup(sf::Vector2f(2.5f, 2.5f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -50.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(255, 255, 255, 150));

	m_puff.setup(sf::Vector2f(2.5f, 2.5f));
	m_puff.setVelocity(sf::Vector2f(-50.f, -60.f));
	m_puff.setEmitTimeRange(0.05f, 0.1f);
	m_puff.setGrowTimeRange(0.4f, 0.6f);
	m_puff.setLifeTimeRange(0.6f, 0.8f);
	m_puff.setScaleFactor(15.f);
	m_puff.setDispersion(120.f);
	m_puff.setColor(sf::Color(255, 255, 255, 150));
}

void ConstanceNpc::setupMachine(void)
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

void ConstanceNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	m_timer += frametime;
	m_puffTimer += frametime;
	if (m_timer > sf::seconds(10.f))
	{
		m_canSmoke = true;
		m_timer = sf::Time::Zero;
	}
	else
		m_canSmoke = false;

	updateState();
	updatePhysics();

	sprite.update(frametime);
	sf::Vector2f const & center = getBox()->getRenderPosition();
	sprite.setPosition(center);
	if (sprite.getCurrentEvent() == Idle)
	{
		m_smoke.setPosition(ANpc::getPosition() + sf::Vector2f(40.f, 100.f));
		m_smoke.setVelocity(sf::Vector2f(0.f, -30.f));
	}
	else
	{
		m_smoke.setPosition(ANpc::getPosition() + sf::Vector2f(-10.f, 50.f));
		m_smoke.setVelocity(sf::Vector2f(-50.f, 0.f));
	}
	if (sprite.isTerminated())
		m_puffTimer = sf::Time::Zero;
	m_smoke.update(frametime);

//	if (m_puffTimer <= m_puffTimerMax)
//		m_puff.setCanEmit(true);
//	else
		m_puff.setCanEmit(false);
	m_puff.setPosition(ANpc::getPosition() + sf::Vector2f(0.f, 43.f));
	m_puff.update(frametime);

	updateText(frametime);
	resetVariables();
}

bool ConstanceNpc::canSpecial1(void) const
{
	if (m_canSmoke == true)
		return true;
	return false;
}

void ConstanceNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle && m_canSmoke)
		sprite.setNextEvent(Special1);
	else if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
		sprite.setNextEvent(Idle);
}

void ConstanceNpc::updatePhysics(void)
{
}

void ConstanceNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
	m_smoke.draw(render);
	m_puff.draw(render);
}
