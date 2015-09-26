#include "CedricNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>
#include <Math.hpp>
#include <Interpolations.hpp>

CedricNpc::CedricNpc(SkyCycle const & skyCycle) :
	ANpc(CEDRIC_OSS),
	m_skyCycle(skyCycle),
	m_prevDayState(skyCycle.isDay()),
	m_shaderIndex(0u),
	m_startBalle(false),
	m_timer(sf::Time::Zero),
	m_effectDuration(sf::seconds(20.f))
{
	setSize(sf::Vector2f(35.f, 100.f));
	setOrigin(sf::Vector2f(75.f, 68.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(0.f, -50.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::CedricNpc), static_cast<std::size_t>(GameObjectType::Player) | static_cast<std::size_t>(GameObjectType::PlayerEvent));

	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	m_shader.loadFromMemory(resources.getText(VISION_TROUBLE_FRAG), sf::Shader::Fragment);
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_shaderIndex = postEffect.addEffect(std::move(postEffectShader));
}

CedricNpc::~CedricNpc(void)
{
	octo::Application::getPostEffectManager().enableEffect(m_shaderIndex, false);
}

void CedricNpc::setup(void)
{
	setupIdleAnimation({
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 7u),
			FramePair(0.4f, 8u),
			FramePair(0.4f, 7u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u)}, octo::LoopMode::Loop);

	setupWalkAnimation({
			FramePair(0.2f, 0u),
			FramePair(0.2f, 1u),
			FramePair(0.2f, 0u),
			FramePair(0.2f, 3u),
			FramePair(0.2f, 4u),
			FramePair(0.2f, 3u),
			FramePair(0.2f, 2u)}, octo::LoopMode::Loop);

	setupSpecial1Animation({
			FramePair(0.2f, 10u),
			FramePair(0.2f, 11u),
			FramePair(0.2f, 12u),
			FramePair(0.2f, 13u),
			FramePair(0.2f, 14u)}, octo::LoopMode::NoLoop);

	setupSpecial2Animation({
			FramePair(0.2f, 15u),
			FramePair(0.2f, 16u),
			FramePair(0.2f, 17u),
			FramePair(0.2f, 18u),
			FramePair(0.2f, 19u),
			FramePair(0.2f, 20u),
			FramePair(0.2f, 21u),
			FramePair(0.2f, 22u)}, octo::LoopMode::NoLoop);

	setupAnimation(m_idleAnimationNight, {
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 32u),
			FramePair(0.4f, 33u),
			FramePair(0.4f, 32u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u)}, octo::LoopMode::Loop);

	setupAnimation(m_walkAnimationNight, {
			FramePair(0.2f, 25u),
			FramePair(0.2f, 26u),
			FramePair(0.2f, 25u),
			FramePair(0.2f, 28u),
			FramePair(0.2f, 29u),
			FramePair(0.2f, 28u),
			FramePair(0.2f, 27u)}, octo::LoopMode::Loop);

	setupAnimation(m_special1AnimationNight, {
			FramePair(0.2f, 35u),
			FramePair(0.2f, 36u),
			FramePair(0.2f, 37u),
			FramePair(0.2f, 38u),
			FramePair(0.2f, 39u)}, octo::LoopMode::NoLoop);

	setupAnimation(m_special2AnimationNight, {
			FramePair(0.2f, 40u),
			FramePair(0.2f, 41u),
			FramePair(0.2f, 42u),
			FramePair(0.2f, 43u),
			FramePair(0.2f, 44u),
			FramePair(0.2f, 45u),
			FramePair(0.2f, 46u),
			FramePair(0.2f, 47u)}, octo::LoopMode::NoLoop);

	setupMachine();
}

void CedricNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					walkLeft;
	StatePtr					walkRight;
	StatePtr					special1;
	StatePtr					special2;
	StatePtr					idleNight;
	StatePtr					walkLeftNight;
	StatePtr					walkRightNight;
	StatePtr					special1Night;
	StatePtr					special2Night;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	walkLeft = std::make_shared<State>("1", getWalkAnimation(), getSprite());
	walkRight = std::make_shared<State>("2", getWalkAnimation(), getSprite());
	special1 = std::make_shared<State>("3", getSpecial1Animation(), getSprite());
	special2 = std::make_shared<State>("4", getSpecial2Animation(), getSprite());
	idleNight = std::make_shared<State>("5", m_idleAnimationNight, getSprite());
	walkLeftNight = std::make_shared<State>("6", m_walkAnimationNight, getSprite());
	walkRightNight = std::make_shared<State>("7", m_walkAnimationNight, getSprite());
	special1Night = std::make_shared<State>("8", m_special1AnimationNight, getSprite());
	special2Night = std::make_shared<State>("9", m_special2AnimationNight, getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, walkLeft, idle);
	machine.addTransition(Idle, walkRight, idle);
	machine.addTransition(Idle, special1, idle);
	machine.addTransition(Idle, special2Night, idle);

	machine.addTransition(Left, idle, walkLeft);
	machine.addTransition(Left, walkRight, walkLeft);
	machine.addTransition(Left, special2Night, walkLeft);

	machine.addTransition(Right, idle, walkRight);
	machine.addTransition(Right, walkLeft, walkRight);

	machine.addTransition(Special1, idle, special1);
	machine.addTransition(Special1, walkLeft, special1);
	machine.addTransition(Special1, walkRight, special1);

	machine.addTransition(Special2, idle, special2);
	machine.addTransition(Special2, walkLeft, special2);
	machine.addTransition(Special2, walkRight, special2);

	machine.addTransition(IdleNight, idleNight, idleNight);
	machine.addTransition(IdleNight, walkLeftNight, idleNight);
	machine.addTransition(IdleNight, walkRightNight, idleNight);
	machine.addTransition(IdleNight, special1Night, idleNight);
	machine.addTransition(IdleNight, special2, idleNight);

	machine.addTransition(LeftNight, idleNight, walkLeftNight);
	machine.addTransition(LeftNight, walkRightNight, walkLeftNight);

	machine.addTransition(RightNight, idleNight, walkRightNight);
	machine.addTransition(RightNight, walkLeftNight, walkRightNight);

	machine.addTransition(Special1Night, idleNight, special1Night);
	machine.addTransition(Special1Night, walkLeftNight, special1Night);
	machine.addTransition(Special1Night, walkRightNight, special1Night);

	machine.addTransition(Special2Night, idleNight, special2Night);
	machine.addTransition(Special2Night, walkLeftNight, special2Night);
	machine.addTransition(Special2Night, walkRightNight, special2Night);

	setMachine(machine);
	setNextEvent(Left);
}

void CedricNpc::startBalle(void)
{
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
	postEffect.enableEffect(m_shaderIndex, true);
	m_startBalle = true;
}

void CedricNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	updateState();
	updatePhysics();

	// Get Night cycle to change the animation
	m_prevDayState = m_skyCycle.isDay();

	sprite.update(frametime);
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();
	sprite.setPosition(bounds.left, bounds.top);

	updateText(frametime);

	if (m_startBalle)
	{
		m_timer += frametime;
		float length;
		if (m_timer < m_effectDuration / 2.f)
			length = octo::linearInterpolation(0.f, 2.f, m_timer / (m_effectDuration / 2.f));
		else
			length = octo::linearInterpolation(2.f, 0.f, (m_timer - m_effectDuration / 2.f) / (m_effectDuration / 2.f));
		sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
		length *= 40.f;
		float rotation = m_timer.asSeconds() / 5.f;
		float x = std::cos(rotation * octo::Pi2 * 1.5f) * length / rect.width;
		float y = std::sin(rotation * octo::Pi2 * 2.f) * length / rect.height;
		float z = std::sin(rotation * octo::Pi2) * length / rect.height;
		m_shader.setParameter("offset", x, y, z);
		if (m_timer > m_effectDuration)
		{
			m_timer = sf::Time::Zero;
			octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();
			postEffect.enableEffect(m_shaderIndex, false);
			m_startBalle = false;
		}
	}
	resetVariables();
}

void CedricNpc::updateState(void)
{
	RectangleShape * box = getBox();
	octo::CharacterSprite & sprite = getSprite();
	sf::FloatRect const & bounds = box->getGlobalBounds();
	sf::FloatRect const & area = getArea();

	if (sprite.getCurrentEvent() == Left && bounds.left <= area.left)
	{
		if (m_prevDayState && m_skyCycle.isNight())
			sprite.setNextEvent(Special2);
		else
		{
			sprite.setNextEvent(Right);
			sprite.setOrigin(getOrigin().x, getOrigin().y);
			sprite.setScale(getScale(), getScale());
		}
	}
	else if (sprite.getCurrentEvent() == Right && (bounds.left + bounds.width) >= (area.left + area.width))
	{
		if (m_prevDayState && m_skyCycle.isNight())
			sprite.setNextEvent(Special2);
		else
		{
			sprite.setNextEvent(Left);
			sf::Vector2f const & size = sprite.getLocalSize();
			sprite.setOrigin(size.x - getOrigin().x, getOrigin().y);
			sprite.setScale(-getScale(), getScale());
		}
	}
	else if (sprite.getCurrentEvent() == Special2)
	{
		if (sprite.isTerminated())
			sprite.setNextEvent(IdleNight);
	}
	else if (sprite.getCurrentEvent() == Special2Night)
	{
		if (sprite.isTerminated())
			sprite.setNextEvent(Left);
	}
	else if (sprite.getCurrentEvent() == IdleNight)
	{
		if (!m_prevDayState && m_skyCycle.isDay())
			sprite.setNextEvent(Special2Night);
	}
}

void CedricNpc::updatePhysics(void)
{
	RectangleShape * box = getBox();
	octo::CharacterSprite & sprite = getSprite();
	sf::Vector2f velocity;

	if (sprite.getCurrentEvent() == Left || sprite.getCurrentEvent() == LeftNight)
	{
		velocity.x = (-1.f * getVelocity());
	}
	else if (sprite.getCurrentEvent() == Right || sprite.getCurrentEvent() == RightNight)
	{
		velocity.x = getVelocity();
	}
	box->setVelocity(velocity);
}
