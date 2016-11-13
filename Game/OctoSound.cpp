#include "OctoSound.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
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
	m_timeDrinkSound(sf::seconds(1.f)),
	m_inWater(false),
	m_onGround(false),
	m_landing(false),
	m_transitionInWater(false),
	m_transitionOutWater(false),
	m_engine(std::time(0)),
	m_pitchDistribution(0.5f, 1.5f)
{
	m_soundFadeOut.reserve(100);
}

CharacterOcto::OctoSound::~OctoSound()
{
	for (auto & sound : m_soundFadeOut)
		sound.sound->stop();
	if (m_sound != nullptr)
		m_sound->stop();
	if (m_soundTransition != nullptr)
		m_soundTransition->stop();
	if (m_soundEnvironment != nullptr)
		m_soundEnvironment->stop();
}

void	CharacterOcto::OctoSound::update(sf::Time frameTime, Events event, bool inWater, bool onGround)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	float						volume = 0.f;

	if (m_soundTransition != nullptr)
	{
		m_timeSoundTransition += frameTime;
		volume = audio.getSoundVolume() * (1.f - (m_timeSoundTransition / m_timeSoundTransitionMax));
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
	Progress const &			progress = Progress::getInstance();

	if (progress.getOctoPos().y > m_waterLevel - 80.f)
	{
		if (!m_inWater && inWater)
			m_transitionInWater = true;
		if (m_inWater && !inWater)
			m_transitionOutWater = true;
	}
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
		audio.playSound(resources.getSound(OCTO_SOUND_JUMP_LANDING_OGG), 1.f);
	if (m_inWater && m_soundEnvironment == nullptr)
	{
		m_soundEnvironment = audio.playSound(resources.getSound(OCTO_SOUND_WATER_BUBBLES_OGG), 1.f);
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
				audio.playSound(resources.getSound(OCTO_SOUND_DOUBLE_JUMP_OGG), 1.f);
			break;
		case DoubleJump:
			audio.playSound(resources.getSound(OCTO_SOUND_DOUBLE_JUMP_OGG), 1.f, m_pitchDistribution(m_engine));
			break;
		case StartJump:
			audio.playSound(resources.getSound(OCTO_SOUND_JUMP_OGG), 1.f, m_pitchDistribution(m_engine));
			break;
		case Death:
			audio.playSound(resources.getSound(OCTO_SOUND_HELMET_OGG), 1.f);
			audio.playSound(resources.getSound(OCTO_VOICE_DEATH_OGG), 1.f);
			break;
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
			if (m_timeEventFall > sf::seconds(1.8f) && !m_inWater && m_sound == nullptr)
				m_sound = audio.playSound(resources.getSound(OCTO_VOICE_FALL_OGG), 1.f);
			break;
		case Idle:
			m_timeEventIdle += frameTime;
			if (m_timeEventIdle > sf::seconds(20.f) && m_sound == nullptr)
				m_sound = audio.playSound(resources.getSound(OCTO_VOICE_MONOLOGUE_OGG), 1.f);
			break;
		case Elevator:
			m_timeEventElevator += frameTime;
			if (m_timeEventElevator > sf::seconds(1.f) && m_sound == nullptr)
				m_sound = audio.playSound(resources.getSound(OCTO_VOICE_ELEVATOR_OGG), 1.f);
			break;
		case SlowFall1:
		case SlowFall2:
		case SlowFall3:
			if (m_sound == nullptr)
			{
				m_sound = audio.playSound(resources.getSound(OCTO_SOUND_SLOW_FALL_OGG), 1.f);
				m_sound->setLoop(true);
			}
			if (m_transitionInWater)
				m_sound->stop();
			break;
		case StartJump:
		case DoubleJump:
		case StartWaterJump:
		case WaterJump:
			break;
		case Drink:
			m_timeDrinkSound -= frameTime;
			if (m_timeDrinkSound <= sf::Time::Zero)
			{
				audio.playSound(resources.getSound(OCTO_SOUND_USE_POTION_OGG), 1.f);
				m_timeDrinkSound = sf::seconds(1.f);
			}
			break;
		default:
			break;
	}
	if (m_transitionInWater)
		audio.playSound(resources.getSound(OCTO_SOUND_WATER_IN_OGG), 1.f, m_pitchDistribution(m_engine));
	else if (m_transitionOutWater)
		audio.playSound(resources.getSound(OCTO_SOUND_WATER_OUT_OGG), 1.f, m_pitchDistribution(m_engine));
}

void	CharacterOcto::OctoSound::walkSound()
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	if (m_sound == nullptr || m_sound->getStatus() == sf::Sound::Stopped)
		m_sound = audio.playSound(resources.getSound(OCTO_SOUND_WALK_ROCK_OGG), 1.f);
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
		volume = (sound.m_maxVolume) * (1.f - (sound.time / sf::seconds(0.5f)));
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
