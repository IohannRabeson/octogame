#include "AudioMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

class MusicYesNo: public YesNoMenu
{
	void setIndex(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		m_isMusic = audio.getMusicEnabled();
		setIndexCursor(m_isMusic);
	}

	void actionYes(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		audio.setMusicEnabled(true);
	}

	void actionNo(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		audio.setMusicEnabled(false);
	}

private:
	bool	m_isMusic;
};

class SoundYesNo: public YesNoMenu
{
	void setIndex(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		m_isSound = audio.getSoundEnabled();
		setIndexCursor(m_isSound);
	}

	void actionYes(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		audio.setSoundEnabled(true);
	}

	void actionNo(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		audio.setSoundEnabled(false);
	}

private:
	bool	m_isSound;
};

AudioMenu::AudioMenu(void)
{
}

void AudioMenu::createMenus(void)
{
	addMenu("Music", new MusicYesNo());
	addMenu("Sound", new SoundYesNo());
}

