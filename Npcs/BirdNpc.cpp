#include "BirdNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "RandomGenerator.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

BirdNpc::BirdNpc(ResourceKey const & npcId) :
	ANpc(npcId, false),
	m_animationEnd(false),
	m_generator("random"),
	m_speedLimit(m_generator.randomFloat(30.f, 150.f)),
	m_flySpeed(sf::Vector2f(m_generator.randomFloat(200.f, 400.f), m_generator.randomFloat(-100.f, -300.f)))
{
	setSize(sf::Vector2f(10.f, 45.f));
	setOrigin(sf::Vector2f(90.f, 27.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(100.f, -80.f));
	setup();

	setTimerMax(sf::seconds(m_generator.randomFloat(5.f, 10.f)));
}

void BirdNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(1.0f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getWalkAnimation().setFrames({
			Frame(sf::seconds(2.0f), {2u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::NoLoop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {5u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	getSpecial2Animation().setFrames({
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(1.f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial2Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void BirdNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					walk;
	StatePtr					special1;
	StatePtr					special2;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	walk = std::make_shared<State>("2", getWalkAnimation(), getSprite());
	special1 = std::make_shared<State>("3", getSpecial1Animation(), getSprite());
	special2 = std::make_shared<State>("4", getSpecial2Animation(), getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, walk, idle);
	machine.addTransition(Idle, special1, idle);

	machine.addTransition(Left, walk, walk);
	machine.addTransition(Left, idle, walk);
	machine.addTransition(Left, special1, walk);

	machine.addTransition(Special1, special1, special1);
	machine.addTransition(Special1, idle, special1);
	machine.addTransition(Special1, special2, special1);

	machine.addTransition(Special2, special2, special2);
	machine.addTransition(Special2, special1, special2);

	setMachine(machine);
	setNextEvent(Idle);
}

void BirdNpc::setPosition(sf::Vector2f const & position)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special2)
		return;
	ANpc::setPosition(position);
}

void BirdNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Left && sprite.isTerminated())
	{
		sprite.setNextEvent(Idle);
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (getCollideEventOcto())
		{
			sprite.setNextEvent(Special1);
			m_startPosition = getPosition();
		}
		else if (getTimer() >= getTimerMax())
		{
			setTimer(sf::Time::Zero);
			setTimerMax(sf::seconds(m_generator.randomFloat(5.f, 10.f)));
			sprite.setNextEvent(Left);
		}
	}
	else if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
		sprite.setNextEvent(Special2);
}

void BirdNpc::update(sf::Time frametime)
{
	addTimer(frametime);
	octo::CharacterSprite & sprite = getSprite();
	octo::Camera const & camera = octo::Application::getCamera();
	sf::Vector2f cameraSize = camera.getSize();
	sf::Vector2f cameraCenter = camera.getCenter();
	float leftLimit = cameraCenter.x - cameraSize.x * 2.f;
	float rightLimit = cameraCenter.x + cameraSize.x * 2.f;

	if (sprite.getCurrentEvent() == Special2 || sprite.getCurrentEvent() == Special1)
	{
		m_startPosition += m_flySpeed  * frametime.asSeconds();
		if (m_startPosition.x >= rightLimit)
			m_startPosition.x = leftLimit;
		else if (m_startPosition.x <= leftLimit)
			m_startPosition.x = rightLimit;
		if (m_flySpeed.y < 0.f)
			m_flySpeed.y += frametime.asSeconds() * m_speedLimit;
	}

	updateState();
	updatePhysics();

	sf::Vector2f const & center = getBox()->getRenderPosition();
	sprite.update(frametime);
	sprite.setPosition(center);

	updateText(frametime);
	resetVariables();
}

void BirdNpc::updatePhysics(void)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special2 || sprite.getCurrentEvent() == Special1)
	{
		RectangleShape * box = getBox();

		box->setPosition(m_startPosition);
		box->update();
	}
}

void BirdNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
}

void BirdNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (!m_animationEnd)
		ANpc::draw(render, states);
}
