#include "FranfranNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

FranfranNpc::FranfranNpc(void) :
	ANpc(FRANFRAN_OSS)
{
	setSize(sf::Vector2f(35.f, 75.f));
	setOrigin(sf::Vector2f(75.f, 170.f));
	setScale(0.6f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
	setup();
}

void FranfranNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()})
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17u, sf::FloatRect(), sf::Vector2f()})
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();

	m_smoke.setup(sf::Vector2f(2.f, 2.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -10.f));
	m_smoke.setGrowTimeRange(0.3f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(2.f);
	m_smoke.setDispersion(30.f);
	m_smoke.setColor(sf::Color(255, 255, 255, 150));
}

void FranfranNpc::setupMachine(void)
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

void FranfranNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	m_timer += frametime;
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
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();
	sprite.setPosition(bounds.left, bounds.top);
	m_smoke.setPosition(ANpc::getPosition() + sf::Vector2f(-15.f, -10.f));
	m_smoke.update(frametime);

	updateText(frametime);
}

bool FranfranNpc::canSpecial1(void) const
{
	if (m_canSmoke == true)
		return true;
	return false;
}

void FranfranNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle && m_canSmoke)
		sprite.setNextEvent(Special1);
	else if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
		sprite.setNextEvent(Idle);
}

void FranfranNpc::updatePhysics(void)
{
	CircleShape * eventBox = getEventBox();
	octo::CharacterSprite & sprite = getSprite();

	eventBox->setPosition(sprite.getPosition().x - eventBox->getRadius(), sprite.getPosition().y - eventBox->getRadius());
}

void FranfranNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	m_smoke.draw(render);
	ANpc::draw(render, states);
}

