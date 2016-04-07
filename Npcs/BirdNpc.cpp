#include "BirdNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "RandomGenerator.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

BirdNpc::BirdNpc(void) :
	ANpc(BIRD_BLUE_OSS),
	m_animationEnd(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::CharacterSprite & sprite = getSprite();
	RandomGenerator generator("random");

	if (generator.randomBool(0.5f))
		sprite.setSpriteSheet(resources.getSpriteSheet(BIRD_BLUE_OSS));
	else
		sprite.setSpriteSheet(resources.getSpriteSheet(BIRD_RED_OSS));
	setSize(sf::Vector2f(10.f, 45.f));
	setOrigin(sf::Vector2f(90.f, 13.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(100.f, -80.f));
	setup();

	m_speedLimit = generator.randomFloat(30.f, 150.f);
	m_flySpeed = sf::Vector2f(generator.randomFloat(200.f, 400.f), generator.randomFloat(-100.f, -300.f));
	setupBox(this, static_cast<std::size_t>(GameObjectType::LucienNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void BirdNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(1.0f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
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

void BirdNpc::setupMachine(void)
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

void BirdNpc::setPosition(sf::Vector2f const & position)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1)
		return;
	ANpc::setPosition(position);
}

void BirdNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle && getCollideEventOcto())
	{
		sprite.setNextEvent(Special1);
		m_startPosition = getPosition();
	}
}

void BirdNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	octo::Camera const & camera = octo::Application::getCamera();
	sf::Vector2f cameraSize = camera.getSize();
	sf::Vector2f cameraCenter = camera.getCenter();
	float leftLimit = cameraCenter.x - cameraSize.x * 2.f;
	float rightLimit = cameraCenter.x + cameraSize.x * 2.f;

	if (sprite.getCurrentEvent() == Special1)
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
	if (sprite.getCurrentEvent() == Special1)
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
