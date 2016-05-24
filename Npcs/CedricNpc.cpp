#include "CedricNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "Progress.hpp"
#include "ChallengeManager.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>
#include <Math.hpp>
#include <Interpolations.hpp>

std::size_t CedricNpc::m_count = 0u;

CedricNpc::CedricNpc(SkyCycle const & skyCycle) :
	ANpc(CEDRIC_OSS),
	m_skyCycle(skyCycle),
	m_prevDayState(skyCycle.isDay()),
	m_id(m_count++)
{
	setSize(sf::Vector2f(200.f, 100.f));
	setOrigin(sf::Vector2f(-30.f, 68.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(80.f, -50.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::CedricNpc), static_cast<std::size_t>(GameObjectType::Player) | static_cast<std::size_t>(GameObjectType::PlayerEvent));

	if (m_id == 0u)
		setCurrentText(0u);
	else
		setCurrentText(3u);
}

CedricNpc::~CedricNpc(void)
{
	m_count = 0u;
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
	if (m_id == 0u)
		ChallengeManager::getInstance().getEffect(ChallengeManager::Effect::Duplicate).start();
}

sf::Time CedricNpc::getEffectDuration(void) const
{
	return ChallengeManager::getInstance().getEffect(ChallengeManager::Effect::Duplicate).getDuration();
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

	if (m_id == 0u && ChallengeManager::getInstance().getEffect(ChallengeManager::Effect::Duplicate).enable())
		setCurrentText(1u);
	resetVariables();
}

void CedricNpc::updateState(void)
{
	RectangleShape * box = getBox();
	octo::CharacterSprite & sprite = getSprite();
	sf::FloatRect const & bounds = box->getGlobalBounds();
	sf::FloatRect const & area = getArea();

	if (sprite.getCurrentEvent() == Left)
	{
		if (m_prevDayState && m_skyCycle.isNight())
		{
			sprite.setNextEvent(Special2);
		}
		else if (bounds.left <= area.left)
		{
			sprite.setNextEvent(Right);
			sprite.setOrigin(getOrigin().x, getOrigin().y);
			sprite.setScale(getScale(), getScale());
		}
	}
	else if (sprite.getCurrentEvent() == Right)
	{
		if (m_prevDayState && m_skyCycle.isNight())
		{
			sprite.setNextEvent(Special2);
		}
		else if ((bounds.left + bounds.width) >= (area.left + area.width))
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
