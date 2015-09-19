#include "OctoSound.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

CharacterOcto::OctoSound::OctoSound() :
	m_sound(nullptr)
{
}

CharacterOcto::OctoSound::~OctoSound()
{
	if (m_sound != nullptr)
		m_sound->stop();
}

void	CharacterOcto::OctoSound::update(sf::Time frameTime, Events event)
{
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
}

void	CharacterOcto::OctoSound::resetTimeEvent()
{
	m_timeEventFall = sf::Time::Zero;
	m_timeEventIdle = sf::Time::Zero;
}

void	CharacterOcto::OctoSound::startEvent(Events event)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();

	switch(event)
	{
		case Elevator:
		case DoubleJump:
			m_sound = audio.playSound(resources.getSound(DOUBLE_JUMP_TEST_WAV));
			break;
		case Death:
			m_sound = audio.playSound(resources.getSound(OCTO_DEATH_WAV));
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
		case Fall:
			m_timeEventFall += frameTime;
			if (m_timeEventFall > sf::seconds(3.f) && m_sound == nullptr)
			{
				m_sound = audio.playSound(resources.getSound(OCTO_FEAR_WAV));
			}
			break;
		case Idle:
			m_timeEventIdle += frameTime;
			if (m_timeEventIdle > sf::seconds(6.f) && m_sound == nullptr)
			{
				m_sound = audio.playSound(resources.getSound(OCTO_MONOLOGUE_WAV));
			}
			break;
		case Right:
		case Left:
			break;
		case Jump:
			break;
		case DoubleJump:
			break;
		case Death:
			break;
		default:
			break;
	}
}
