#include "OctoSound.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

CharacterOcto::OctoSound::OctoSound() :
	m_sound(nullptr),
	m_prevEvent(Idle),
	m_inWater(false),
	m_onGround(false),
	m_landing(false),
	m_transitionWater(false),
	m_volumeEffect(0.5f),
	m_volumeVoice(0.7f),
	m_engine(std::time(0)),
	m_pitchDistribution(0.5f, 1.5f)
{
}

CharacterOcto::OctoSound::~OctoSound()
{
	if (m_sound != nullptr)
		m_sound->stop();
}

void	CharacterOcto::OctoSound::update(sf::Time frameTime, Events event, bool inWater, bool onGround)
{
	if (m_inWater != inWater)
		m_transitionWater = true;
	m_inWater = inWater;
	if (!m_onGround && onGround)
		m_landing = true;
	m_onGround = onGround;
	transition();
	if (m_prevEvent != event)
	{
		if (m_sound != nullptr){
			m_sound->stop();
			m_sound = nullptr;
		}
		resetTimeEvent();
		startEvent(event);
	}
	else
		duringEvent(frameTime, event);
	m_prevEvent = event;
	m_transitionWater = false;
	m_landing = false;
}

void	CharacterOcto::OctoSound::resetTimeEvent()
{
	m_timeEventFall = sf::Time::Zero;
	m_timeEventIdle = sf::Time::Zero;
	m_timeEventElevator = sf::Time::Zero;
}

void	CharacterOcto::OctoSound::transition()
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	if (m_landing)
		audio.playSound(resources.getSound(OCTO_JUMP_LANDING_WAV), m_volumeEffect);
	if (m_transitionWater)
		audio.playSound(resources.getSound(OCTO_WALK_WATER_WAV), m_volumeEffect);
}

void	CharacterOcto::OctoSound::startEvent(Events event)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	switch(event)
	{
		case StartElevator:
			if (m_onGround)
				audio.playSound(resources.getSound(DOUBLE_JUMP_TEST_WAV), m_volumeEffect);
			break;
		case DoubleJump:
			audio.playSound(resources.getSound(DOUBLE_JUMP_TEST_WAV), m_volumeEffect,
					m_pitchDistribution(m_engine));
			break;
		case Death:
			m_sound = audio.playSound(resources.getSound(OCTO_DEATH_WAV), m_volumeVoice);
		default:
			break;
	}
}
void	CharacterOcto::OctoSound::duringEvent(sf::Time frameTime, Events event)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	switch(event)
	{
		case Right:
		case Left:
				walkSound();
				break;
		case Fall:
			m_timeEventFall += frameTime;
			if (m_timeEventFall > sf::seconds(3.f) && m_sound == nullptr)
			{
				m_sound = audio.playSound(resources.getSound(OCTO_FEAR_WAV), m_volumeVoice);
			}
			break;
		case Idle:
			m_timeEventIdle += frameTime;
			if (m_timeEventIdle > sf::seconds(6.f) && m_sound == nullptr)
			{
				m_sound = audio.playSound(resources.getSound(OCTO_MONOLOGUE_WAV), m_volumeVoice);
			}
			break;
		case Elevator:
			m_timeEventElevator += frameTime;
			if (m_timeEventElevator > sf::seconds(1.f) && m_sound == nullptr)
				m_sound = audio.playSound(resources.getSound(OCTO_START_ELEVATOR_WAV), m_volumeEffect);
			break;
		case SlowFall:
			if (m_sound == nullptr)
			{
				m_sound = audio.playSound(resources.getSound(OCTO_SLOWFALL_WAV), m_volumeEffect);
				m_sound->setLoop(true);
			}
			if (m_inWater)
				m_sound->stop();
		default:
			break;
	}
}

void	CharacterOcto::OctoSound::walkSound()
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	Level						level = Progress::getInstance().getNextDestination();

	if (m_inWater && (m_sound == nullptr || m_sound->getStatus() == sf::Sound::Stopped))
	{
		m_sound = audio.playSound(resources.getSound(OCTO_WALK_WATER_WAV), m_volumeEffect);
		return;
	}

	switch (level)
	{
		case Level::LevelTwo:
			if (m_sound == nullptr || m_sound->getStatus() == sf::Sound::Stopped)
				m_sound = audio.playSound(resources.getSound(OCTO_WALK_ROCK_WAV), m_volumeEffect);
			break;
		case Level::LevelThree:
			if (m_sound == nullptr || m_sound->getStatus() == sf::Sound::Stopped)
				m_sound = audio.playSound(resources.getSound(OCTO_WALK_LEAF_WAV), m_volumeEffect);
			break;
		default:
			break;
	}
}
