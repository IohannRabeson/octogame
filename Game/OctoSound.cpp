#include "OctoSound.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

CharacterOcto::OctoSound::OctoSound() :
	m_sound(nullptr)
{
}

void	CharacterOcto::OctoSound::update(sf::Time frameTime, Events event)
{
	octo::AudioManager&		audio = octo::Application::getAudioManager();
	octo::ResourceManager&	resources = octo::Application::getResourceManager();

	if (m_prevEvent == event)
		return;
	if (m_sound != nullptr)
		m_sound->stop();
	switch(event)
	{
		case Idle:
			m_sound = audio.playSound(resources.getSound(OCTO_MONOLOGUE_WAV));
			break;
		case Right:
		case Left:
			break;
		case Jump:
			break;
		case DoubleJump:
			break;
		case Death:
			m_sound = audio.playSound(resources.getSound(OCTO_DEATH_WAV));
		default:
			break;
	}
	m_prevEvent = event;
	(void)frameTime;
}
