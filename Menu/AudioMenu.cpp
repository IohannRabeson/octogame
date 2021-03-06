#include "AudioMenu.hpp"
#include "SoundVolumeMenu.hpp"
#include "MusicVolumeMenu.hpp"
#include "GlobalVolumeMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

class MusicYesNo: public YesNoMenu
{
	void setIndex(void)
	{
		m_volumeMusic = 9;
		m_isActive = false;
		setIndexCursor(m_isActive);
	}

	void actionYes(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		m_volumeMusic = audio.getMusicVolume();
		audio.setMusicVolume(40);
		Progress::getInstance().setMusicVolume(40);
	}

	void actionNo(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		audio.setMusicVolume(m_volumeMusic);
		Progress::getInstance().setMusicVolume(m_volumeMusic);
	}

private:
	float		m_volumeMusic;
	bool		m_isActive;
};

AudioMenu::AudioMenu(void)
{
}

void AudioMenu::createMenus(void)
{
	addMenu(AMenu::getText("options_audio_global"), std::unique_ptr<GlobalVolumeMenu>(new GlobalVolumeMenu()));
	addMenu(AMenu::getText("options_audio_music"), std::unique_ptr<MusicVolumeMenu>(new MusicVolumeMenu()));
	addMenu(AMenu::getText("options_audio_sound"), std::unique_ptr<SoundVolumeMenu>(new SoundVolumeMenu()));
	//addMenu(L"I <3", std::unique_ptr<MusicYesNo>(new MusicYesNo()));
}

