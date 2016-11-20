#include "CedricWalkNpc.hpp"
#include "Progress.hpp"
#include "RectangleShape.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>

CedricWalkNpc::CedricWalkNpc(void) :
	AWalkNpc(CEDRIC_WALK_OSS),
	m_state(WalkToOcto),
	m_timerSpeak(sf::seconds(4.f)),
	m_timerWalkToRocket(sf::seconds(5.2f)),
	m_drinkSoundTimer(sf::seconds(1.0f))
{
	setType(GameObjectType::CedricWalkNpc);
	setSize(sf::Vector2f(34.f, 164.f));
	setOrigin(sf::Vector2f(84.f, 31.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -20.f));
	setVelocity(200.f);
	setup();
}

void CedricWalkNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::NoLoop);

	getWalkAnimation().setFrames({
			Frame(sf::seconds(0.25f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {2u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {45u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {46u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {47u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	getSpecial2Animation().setFrames({
			Frame(sf::seconds(0.4f), {30u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {31u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {32u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {33u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {32u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {31u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial2Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void CedricWalkNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
	if (m_state == WalkToOcto)
		m_state = Speak;
}

void CedricWalkNpc::update(sf::Time frametime)
{
	ANpc::update(frametime);
	octo::CharacterSprite & sprite = getSprite();
	Progress const & progress = Progress::getInstance();

	switch (m_state)
	{
		case WalkToOcto:
		{
			if (sprite.getCurrentEvent() != Right && sprite.isTerminated())
			{
				reverseSprite(false);
				sprite.setNextEvent(Right);
			}
			break;
		}
		case Speak:
		{
			m_timerSpeak -= frametime;
			if (sprite.getCurrentEvent() != Idle || sprite.isTerminated())
				sprite.setNextEvent(Idle);

			if (m_timerSpeak <= sf::seconds(1.f))
				reverseSprite(true);
			if (m_timerSpeak <= sf::Time::Zero)
			{
				setDisplayText(false);
				m_state = WalkToRocket;
			}
			break;
		}
		case WalkToRocket:
		{
			m_timerWalkToRocket -= frametime;
			if (sprite.getCurrentEvent() != Left && sprite.isTerminated())
			{
				reverseSprite(true);
				sprite.setNextEvent(Left);
			}
			if (m_timerWalkToRocket <= sf::Time::Zero)
			{
				setCurrentText(1u);
				m_state = Stop;
			}
			break;
		}
		case Stop:
		{
			m_drinkSoundTimer -= frametime;
			if (sprite.getCurrentEvent() != Special1)
				sprite.setNextEvent(Special1);
			if (m_drinkSoundTimer <= sf::Time::Zero)
			{
				octo::AudioManager& audio = octo::Application::getAudioManager();
				octo::ResourceManager& resources = octo::Application::getResourceManager();
				audio.playSound(resources.getSound(OCTO_SOUND_USE_POTION_OGG), 1.f, 1.f);
				m_drinkSoundTimer = sf::seconds(1000.f);
			}
			if (sprite.isTerminated())
			{
				sprite.setNextEvent(Idle);
				setDisplayText(true);
				m_state = Wait;
			}
			break;
		}
		case Wait:
		{
			float dist = std::abs(progress.getOctoPos().x - getBox()->getRenderPosition().x);
			bool isRight = true;
			if (progress.getOctoPos().x - getBox()->getRenderPosition().x < 0.f)
				isRight = false;

			if (dist > 200.f)
			{
				if (isRight)
				{
					setCurrentText(2u);
					setDisplayText(true);
					reverseSprite(false);
					if (sprite.getCurrentEvent() != Right)
						sprite.setNextEvent(Right);
				}
				else
				{
					setDisplayText(false);
					reverseSprite(true);
					if (sprite.getCurrentEvent() != Left)
						sprite.setNextEvent(Left);
				}
			}
			else
			{
				if (sprite.getCurrentEvent() != Idle || sprite.isTerminated())
					sprite.setNextEvent(Idle);
			}

			break;
		}
		default:
			break;
	}

}

void CedricWalkNpc::updateState(void)
{
}
