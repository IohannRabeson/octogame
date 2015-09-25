#include "LucienNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

LucienNpc::LucienNpc(void) :
	ANpc(LUCIEN_OSS),
	m_side(true)
{
	setSize(sf::Vector2f(25.f, 75.f));
	setOrigin(sf::Vector2f(90.f, 100.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
	setTimerMax(sf::seconds(10.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::LucienNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void LucienNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {23u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	getSpecial2Animation().setFrames({
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {23u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial2Animation().setLoop(octo::LoopMode::NoLoop);
	setupMachine();
}

void LucienNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					special1State;
	StatePtr					special2State;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1State = std::make_shared<State>("1", getSpecial1Animation(), getSprite());
	special2State = std::make_shared<State>("2", getSpecial2Animation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, special1State, idleState);
	machine.addTransition(Idle, special2State, idleState);

	machine.addTransition(Special1, idleState, special1State);

	machine.addTransition(Special2, idleState, special2State);

	setMachine(machine);
	setNextEvent(Idle);
}

void LucienNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Special1)
	{
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(Idle);
			octo::CharacterSprite & sprite = getSprite();
			sf::Vector2f const & size = sprite.getLocalSize();
			sprite.setOrigin(size.x - getOrigin().x, getOrigin().y);
			sprite.setScale(-getScale(), getScale());
			addTimer(-getTimer());
			m_side = false;
		}
	}
	else if (sprite.getCurrentEvent() == Special2)
	{
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(Idle);
			octo::CharacterSprite & sprite = getSprite();
			sprite.setOrigin(getOrigin().x, getOrigin().y);
			sprite.setScale(getScale(), getScale());
			addTimer(-getTimer());
			m_side = true;
		}
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (getTimer() >= getTimerMax())
		{
			addTimer(-getTimerMax());
			if (m_side)
				sprite.setNextEvent(Special1);
			else
				sprite.setNextEvent(Special2);
		}
	}
}
