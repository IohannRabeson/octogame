#include "MusicPlayer.hpp"
#include "CharacterOcto.hpp"
#include "MapInstance.hpp"
#include <Application.hpp>

#include "ResourceDefinitions.hpp"

MusicPlayer::MusicPlayer(void) :
	m_audio(octo::Application::getAudioManager()),
	m_resource(octo::Application::getResourceManager()),
	m_isPlayingOpusI(false),
	m_isPlayingBallade(false)
{
}

void MusicPlayer::update(CharacterOcto & octo)
{
	if (octo.getPosition().y < MapInstance::HeightOffset * Tile::TileSize && m_isPlayingBallade == false)
	{
		m_isPlayingBallade = true;
		m_isPlayingOpusI = false;
		m_audio.startMusic(m_resource.getSound(BALLADE_MENTALE_WAV), sf::milliseconds(1.f), false);
	}
	else if (octo.getPosition().y >= MapInstance::HeightOffset * Tile::TileSize && m_isPlayingOpusI == false)
	{
		m_isPlayingOpusI = true;
		m_isPlayingBallade = false;
		m_audio.startMusic(m_resource.getSound(MENU_OPUS_I_WAV), sf::milliseconds(1.f), false);
	}
}
