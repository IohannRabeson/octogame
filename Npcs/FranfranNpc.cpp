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
	setTextOffset(sf::Vector2f(0.f, -50.f));
	setup();
}

void FranfranNpc::setup(void)
{
	setupIdleAnimation({
			FramePair(0.3f, 0u),
			FramePair(0.3f, 1u),
			FramePair(0.3f, 2u),
			FramePair(0.3f, 3u),
			FramePair(0.3f, 4u),
			FramePair(0.3f, 3u),
			FramePair(0.3f, 2u),
			FramePair(0.3f, 1u)}, octo::LoopMode::Loop);

	setupWalkAnimation({
			FramePair(0.5f, 0u),
			FramePair(0.5f, 1u),
			FramePair(0.5f, 2u),
			FramePair(0.5f, 3u),
			FramePair(0.5f, 4u),
			FramePair(0.5f, 3u),
			FramePair(0.5f, 2u),
			FramePair(0.5f, 1u)}, octo::LoopMode::Loop);

	setupSpecial1Animation({
			FramePair(0.3f, 6u),
			FramePair(0.3f, 7u),
			FramePair(0.3f, 8u),
			FramePair(0.3f, 9u),
			FramePair(0.3f, 10u),
			FramePair(0.3f, 11u),
			FramePair(0.3f, 12u),
			FramePair(0.3f, 13u),
			FramePair(0.3f, 14u),
			FramePair(0.3f, 15u),
			FramePair(0.3f, 16u),
			FramePair(0.3f, 17u)}, octo::LoopMode::NoLoop);

	setupMachine();
}

void FranfranNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					walkLeft;
	StatePtr					walkRight;
	StatePtr					special1;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	walkLeft = std::make_shared<State>("1", getWalkAnimation(), getSprite());
	walkRight = std::make_shared<State>("2", getWalkAnimation(), getSprite());
	special1 = std::make_shared<State>("3", getSpecial1Animation(), getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, walkLeft, idle);
	machine.addTransition(Idle, walkRight, idle);
	machine.addTransition(Idle, special1, idle);

	machine.addTransition(Left, idle, walkLeft);
	machine.addTransition(Left, walkRight, walkLeft);

	machine.addTransition(Right, idle, walkRight);
	machine.addTransition(Right, walkLeft, walkRight);

	machine.addTransition(Special1, special1, special1);
	machine.addTransition(Special1, idle, special1);
	machine.addTransition(Special1, walkLeft, special1);
	machine.addTransition(Special1, walkRight, special1);

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
	RectangleShape * box = getBox();
	octo::CharacterSprite & sprite = getSprite();
	sf::FloatRect const & bounds = box->getGlobalBounds();
	sf::FloatRect const & area = getArea();

	if (sprite.getCurrentEvent() == Idle && m_canSmoke)
		sprite.setNextEvent(Special1);
	else if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
		sprite.setNextEvent(Idle);

	if (canWalk())
	{
		if (sprite.getCurrentEvent() == Left && bounds.left <= area.left)
		{
			sprite.setNextEvent(Right);
			sprite.setOrigin(getOrigin().x, getOrigin().y);
			sprite.setScale(getScale(), getScale());
		}
		else if (sprite.getCurrentEvent() == Right && (bounds.left + bounds.width) >= (area.left + area.width))
		{
			sprite.setNextEvent(Left);
			sf::Vector2f const & size = sprite.getLocalSize();
			sprite.setOrigin(size.x - getOrigin().x, getOrigin().y);
			sprite.setScale(-getScale(), getScale());
		}
	}
}

void FranfranNpc::updatePhysics(void)
{
	RectangleShape * box = getBox();
	CircleShape * eventBox = getEventBox();
	octo::CharacterSprite & sprite = getSprite();
	sf::Vector2f velocity;

	if (sprite.getCurrentEvent() == Left)
	{
		velocity.x = (-1.f * getVelocity());
	}
	else if (sprite.getCurrentEvent() == Right)
	{
		velocity.x = getVelocity();
	}
	box->setVelocity(velocity);
	eventBox->setPosition(sprite.getPosition().x - eventBox->getRadius(), sprite.getPosition().y - eventBox->getRadius());
}
