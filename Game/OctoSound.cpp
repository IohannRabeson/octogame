#include "OctoSound.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

CharacterOcto::OctoSound::OctoSound() :
	m_sound(nullptr),
	m_soundEnvironment(nullptr),
	m_soundTransition(nullptr),
	m_prevEvent(Idle),
	m_timeEventFall(sf::Time::Zero),
	m_timeEventIdle(sf::Time::Zero),
	m_timeEventElevator(sf::Time::Zero),
	m_timeSoundIn(sf::Time::Zero),
	m_timeSoundTransition(sf::Time::Zero),
	m_timeSoundTransitionMax(sf::seconds(1.f)),
	m_inWater(false),
	m_onGround(false),
	m_landing(false),
	m_transitionInWater(false),
	m_transitionOutWater(false),
	m_volumeEffect(0.5f),
	m_volumeVoice(0.7f),
	m_engine(std::time(0)),
	m_pitchDistribution(0.5f, 1.5f)
{
}

CharacterOcto::OctoSound::~OctoSound()
{
	if (m_sound != nullptr)
		stopSound();
	if (m_soundTransition != nullptr)
		m_soundTransition->stop();
	if (m_soundEnvironment != nullptr)
		m_soundEnvironment->stop();
}

void	CharacterOcto::OctoSound::update(sf::Time frameTime, Events event, bool inWater, bool onGround)
{
	float	volume = 0.f;
	if (m_soundTransition != nullptr)
	{
		m_timeSoundTransition += frameTime;
		volume = (m_volumeEffect * 100) * (1.f - (m_timeSoundTransition / m_timeSoundTransitionMax));
		if (volume >= 0.f)
			m_soundTransition->setVolume(volume);
		if (m_soundTransition->getStatus() == sf::Sound::Stopped || volume < 0.f)
		{
			m_timeSoundTransition = sf::Time::Zero;
			m_soundTransition = nullptr;
		}
	}
	environmentEvent(inWater, onGround);
	if (m_prevEvent != event)
	{
		if (m_sound != nullptr)
			stopSound();
		resetTimeEvent();
		startEvent(event);
	}
	else
		duringEvent(frameTime, event);
	fadeOut(frameTime);
	m_prevEvent = event;
	m_transitionInWater = false;
	m_transitionOutWater = false;
	m_landing = false;
}

void	CharacterOcto::OctoSound::resetTimeEvent()
{
	m_timeEventFall = sf::Time::Zero;
	m_timeEventIdle = sf::Time::Zero;
	m_timeEventElevator = sf::Time::Zero;
}

void	CharacterOcto::OctoSound::environmentEvent(bool inWater, bool onGround)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	if (!m_inWater && inWater)
		m_transitionInWater = true;
	if (m_inWater && !inWater)
		m_transitionOutWater = true;
	m_inWater = inWater;
	if (!m_onGround && onGround)
		m_landing = true;
	m_onGround = onGround;

	if (!m_inWater && m_soundEnvironment != nullptr)
	{
		m_soundEnvironment->stop();
		m_soundEnvironment = nullptr;
	}

	if (m_landing)
		audio.playSound(resources.getSound(OCTO_JUMP_LANDING_WAV), m_volumeEffect);
	if (m_inWater && m_soundEnvironment == nullptr)
	{
		m_soundEnvironment = audio.playSound(resources.getSound(WATER_BUBBLES_WAV));
		m_soundEnvironment->setLoop(true);
	}
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
			audio.playSound(resources.getSound(EXPLODE_HELMET_WAV), m_volumeEffect);
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
			if (m_transitionInWater)
				audio.playSound(resources.getSound(PLOUF_WAV), m_volumeEffect, m_pitchDistribution(m_engine));
			break;
		case Idle:
			m_timeEventIdle += frameTime;
			if (m_timeEventIdle > sf::seconds(12.f) && m_sound == nullptr)
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
			if (m_transitionInWater)
			{
				m_sound->stop();
				audio.playSound(resources.getSound(PLOUF_WAV), m_volumeEffect, m_pitchDistribution(m_engine));
			}
			break;
		case StartJump:
		case Jump:
		case DoubleJump:
		case StartWaterJump:
		case WaterJump:
			if (m_transitionInWater || m_transitionOutWater)
				audio.playSound(resources.getSound(PLOUF_WAV), m_volumeEffect, m_pitchDistribution(m_engine));
			break;
		default:
			break;
	}
}

void	CharacterOcto::OctoSound::walkSound()
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	Level						level = Progress::getInstance().getNextDestination();

	if (m_transitionInWater && m_soundTransition == nullptr)
		m_soundTransition = audio.playSound(resources.getSound(TRANSITION_IN_WATER_WAV), m_volumeEffect);
	if (m_transitionOutWater && m_soundTransition == nullptr)
		m_soundTransition = audio.playSound(resources.getSound(TRANSITION_OUT_WATER_WAV), m_volumeEffect);
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

void	CharacterOcto::OctoSound::stopSound()
{
	m_soundFadeOut.push_back(soundFade(m_sound));
	m_timeSoundIn = sf::Time::Zero;
	m_sound = nullptr;
}

void	CharacterOcto::OctoSound::fadeOut(sf::Time frameTime)
{
	float	volume;
	for (auto & sound : m_soundFadeOut)
	{
		volume = 0.f;
		sound.time += frameTime;
		volume = (sound.m_maxVolume * 50) * (1.f - (sound.time / sf::seconds(0.2f)));
		if (volume >= 0.f)
		{
			sound.sound->setVolume(volume);
		}
		else
		{
			sound.stop = true;
			sound.sound->stop();
		}
	}
	m_soundFadeOut.erase(std::remove_if(m_soundFadeOut.begin() , m_soundFadeOut.end(),
				[](soundFade & p){ return p.stop; }),
			m_soundFadeOut.end());
}
