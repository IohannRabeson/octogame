#include "LucienNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>
#include "ResourceDefinitions.hpp"

LucienNpc::LucienNpc(void) :
	AUniqueNpc(LUCIEN_OSS),
	m_side(true),
	m_sound(true)
{
	setType(GameObjectType::LucienNpc);
	setSize(sf::Vector2f(160.f, 168.f));
	setOrigin(sf::Vector2f(69.f, 25.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
	setTimerMax(sf::seconds(10.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
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

void LucienNpc::playSound(void)
{
	if (m_sound)
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resources = octo::Application::getResourceManager();

		audio.playSound(resources.getSound(OBJECT_LU_OGG), 1.f, 1.f, sf::Vector3f(getBox()->getBaryCenter().x, getBox()->getBaryCenter().y, 0.f), 500.f, 40.f);
		m_sound = false;
	}
}

void LucienNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Special1)
	{
		playSound();
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(Idle);
			reverseSprite(true);
			addTimer(-getTimer());
			m_side = false;
			m_sound = true;
		}
	}
	else if (sprite.getCurrentEvent() == Special2)
	{
		playSound();
		if (sprite.isTerminated())
		{
			sprite.setNextEvent(Idle);
			reverseSprite(false);
			addTimer(-getTimer());
			m_side = true;
			m_sound = true;
		}
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (getTimer() >= getTimerMax())
		{
			addTimer(-getTimerMax());
			if (m_side)
			{
				sprite.setNextEvent(Special1);
				setTextOffset(sf::Vector2f(92.f, -80.f));
			}
			else
			{
				sprite.setNextEvent(Special2);
				setTextOffset(sf::Vector2f(-20.f, -80.f));
			}
		}
	}
}
