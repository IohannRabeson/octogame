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
		setIndexCursor(audio.getMusicEnabled());
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
};

class SoundYesNo: public YesNoMenu
{
	void setIndex(void)
	{
		octo::AudioManager & audio = octo::Application::getAudioManager();
		setIndexCursor(audio.getSoundEnabled());
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
};

AudioMenu::AudioMenu(void)
{
}

void AudioMenu::createMenus(void)
{
	addMenu("Music", std::unique_ptr<MusicYesNo>(new MusicYesNo()));
	addMenu("Sound", std::unique_ptr<SoundYesNo>(new SoundYesNo()));
}

