#include "AudioMenu.hpp"

//TODO: Remove
#include "ControlMenu.hpp"

AudioMenu::AudioMenu(void)
{
}

void AudioMenu::createMenus(void)
{
	addMenu("Music", new ControlMenu());
	addMenu("Sound", new ControlMenu());
}

