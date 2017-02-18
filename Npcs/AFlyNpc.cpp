#include "AFlyNpc.hpp"
#include "RectangleShape.hpp"
#include "CircleShape.hpp"
#include "RandomGenerator.hpp"
#include "CharacterOcto.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

RandomGenerator AFlyNpc::m_generator("random");

AFlyNpc::AFlyNpc(ResourceKey const & npcId, bool isMeetable) :
	ANpc(npcId, isMeetable),
	m_animationEnd(false),
	m_speedLimit(m_generator.randomFloat(30.f, 150.f)),
	m_flySpeed(sf::Vector2f(m_generator.randomFloat(200.f, 400.f), m_generator.randomFloat(-100.f, -300.f)))
{
	setSize(sf::Vector2f(38.f, 58.f));
	setOrigin(sf::Vector2f(75.f, 30.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(28.f, 0.f));
	setup();

	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	m_shader.loadFromMemory(resources.getText(HUE_FRAG), sf::Shader::Fragment);
	m_shader.setUniform("texture", sf::Shader::CurrentTexture);
	m_shader.setUniform("hue", 0.0f);
	setTimerMax(sf::seconds(m_generator.randomFloat(5.f, 10.f)));

	setupBox(this, static_cast<std::size_t>(GameObjectType::FlyNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void AFlyNpc::setup(void)
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
			Frame(sf::seconds(m_generator.randomFloat(0.5f, 2.5f)), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial2Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void AFlyNpc::setupMachine(void)
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

void AFlyNpc::setPosition(sf::Vector2f const & position)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special2)
		return;
	ANpc::setPosition(position);
}

void AFlyNpc::updateState(void)
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

void AFlyNpc::computeFlight(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	octo::Camera const & camera = octo::Application::getCamera();
	sf::Vector2f cameraSize = camera.getSize();
	sf::Vector2f cameraCenter = camera.getCenter();
	float leftLimit = cameraCenter.x - cameraSize.x * 2.f;
	float rightLimit = cameraCenter.x + cameraSize.x * 2.f;

	if (sprite.getCurrentEvent() == Special2)
	{
		m_startPosition.x += m_flySpeed.x  * frametime.asSeconds();
		if (m_startPosition.x >= rightLimit)
			m_startPosition.x = leftLimit;
		else if (m_startPosition.x <= leftLimit)
			m_startPosition.x = rightLimit;
		if (m_flySpeed.y < 0.f)
		{
			m_flySpeed.y += frametime.asSeconds() * m_speedLimit;
			m_startPosition.y += m_flySpeed.y  * frametime.asSeconds();
		}
	}
}

void AFlyNpc::update(sf::Time frametime)
{
	addTimer(frametime);
	computeFlight(frametime);

	updateState();
	updatePhysics();

	octo::CharacterSprite & sprite = getSprite();
	sprite.update(frametime);
	sprite.setPosition(m_startPosition);

	updateText(frametime);
	resetVariables();
}

void AFlyNpc::updatePhysics(void)
{
	octo::CharacterSprite & sprite = getSprite();
	RectangleShape * box = getBox();

	if (sprite.getCurrentEvent() == Special2 || sprite.getCurrentEvent() == Special1)
		box->setPosition(m_startPosition);
	else
		m_startPosition = box->getPosition();
	box->update();
}

void AFlyNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
	m_octoPosition = octo->getPosition();
	if (m_octoPosition.y > m_startPosition.y && m_flySpeed.y > 0.f)
		m_flySpeed.y = m_generator.randomFloat(-80.f, -160.f);
	if (m_octoPosition.x > m_startPosition.x && m_flySpeed.x > 0.f && m_generator.randomBool(0.01f))
	{
		octo::CharacterSprite & sprite = getSprite();
		sf::Vector2f const & size = sprite.getLocalSize();
		sprite.setOrigin(size.x - getOrigin().x, getOrigin().y);
		sprite.setScale(-getScale(), getScale());
		m_flySpeed.x = -m_flySpeed.x;
	}
	else if (m_octoPosition.x < m_startPosition.x && m_flySpeed.x < 0.f && m_generator.randomBool(0.01f))
	{
		octo::CharacterSprite & sprite = getSprite();
		sprite.setOrigin(getOrigin().x, getOrigin().y);
		sprite.setScale(getScale(), getScale());
		m_flySpeed.x = -m_flySpeed.x;
	}
}

void AFlyNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	states.shader = &m_shader;
	if (!m_animationEnd)
		ANpc::draw(render, states);
}
