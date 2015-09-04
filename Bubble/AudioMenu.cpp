#include "AudioMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"


AudioMenu::AudioMenu(void)
{
}

void AudioMenu::createMenus(void)
{
	addMenu("Music", new EmptyMenu());
	addMenu("Sound", new EmptyMenu());
}

