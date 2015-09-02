#include "OptionMenu.hpp"

OptionMenu::OptionMenu(void)
{
}

void OptionMenu::createMenus(void)
{
	addMenu("Audio", new AudioMenu());
	addMenu("Video", new VideoMenu());
	addMenu("Control", new ControlMenu());
	//setCharacterSize();
}
