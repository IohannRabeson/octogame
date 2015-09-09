#include "MusicPlayer.hpp"
#include "CharacterOcto.hpp"
#include "MapInstance.hpp"
#include <Application.hpp>

MusicPlayer::MusicPlayer(void) :
	m_audio(octo::Application::getAudioManager()),
	m_resource(octo::Application::getResourceManager()),
	m_mainMusicOn(false),
	m_currentMainKey(MENU_OPUS_II_WAV),
	m_eventMusicOn(false)
{
	m_mainMusics.resize(3);
	initMusicValue(m_mainMusics[0], MENU_OPUS_I_WAV);
	initMusicValue(m_mainMusics[1], MENU_OPUS_II_WAV);
	initMusicValue(m_mainMusics[2], MENU_OPUS_III_WAV);

	m_eventMusics.resize(1);
	initMusicValue(m_eventMusics[0], BALLADE_MENTALE_WAV);
}

void MusicPlayer::initMusicValue(MusicValue & music, ResourceKey key)
{
	music.key = key;
	music.soundBuffer = m_resource.getSound(key);
	music.duration = music.soundBuffer.getDuration();
	music.offset = sf::Time::Zero;
}

ResourceKey const & MusicPlayer::getOpusMusic(ResourceKey const & key)
{
	//TODO: Random followig music?
	if (key == MENU_OPUS_III_WAV)
		return (MENU_OPUS_I_WAV);
	else if (key == MENU_OPUS_I_WAV)
		return (MENU_OPUS_II_WAV);
	else if (key == MENU_OPUS_II_WAV)
		return (MENU_OPUS_III_WAV);
	return (MENU_OPUS_I_WAV);
}

bool MusicPlayer::getEvent(sf::Vector2f const & octoPos)
{
	if (octoPos.y < MapInstance::HeightOffset * Tile::TileSize)
	{
		m_currentEventKey = BALLADE_MENTALE_WAV;
		return true;
	}
	return false;
}

void MusicPlayer::playMainMusic(sf::Time frameTime, std::vector<MusicValue> & musics, bool & startStateOn, bool & endStateOn, ResourceKey & current)
{
	MusicValue * musicToPlay = nullptr;

	endStateOn = false;
	for (auto &music : musics)
	{
		if (music.key == current)
		{
			music.offset += frameTime;
			musicToPlay = &music;
			if (music.offset >= music.duration && current != BALLADE_MENTALE_WAV)
			{
				music.offset = sf::Time::Zero;
				startStateOn = false;
				//TODO: Find another way to put more events without listing them here
				if (current != BALLADE_MENTALE_WAV)
					current = getOpusMusic(music.key);
			}
		}
	}
	if (startStateOn == false)
	{
		m_audio.startMusic(musicToPlay->soundBuffer, sf::milliseconds(400.f), musicToPlay->offset);
		startStateOn = true;
	}
}

void MusicPlayer::update(sf::Time frameTime, sf::Vector2f const & octoPos)
{
	if (getEvent(octoPos))
		playMainMusic(frameTime, m_eventMusics, m_eventMusicOn, m_mainMusicOn, m_currentEventKey);
	else
		playMainMusic(frameTime, m_mainMusics, m_mainMusicOn, m_eventMusicOn, m_currentMainKey);
}

