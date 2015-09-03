#include "AudioMenu.hpp"
#include "YesNoMenu.hpp"

AudioMenu::AudioMenu(void)
{
}

void AudioMenu::createMenus(void)
{
	addMenu("Music", new YesNoMenu());
	addMenu("Sound", new YesNoMenu());
}

