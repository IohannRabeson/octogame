#include "OptionMenu.hpp"

OptionMenu::OptionMenu(void)
{
}

void OptionMenu::createMenus(void)
{
	addMenu("Audio", std::unique_ptr<AudioMenu>(new AudioMenu()));
	addMenu("Video", std::unique_ptr<VideoMenu>(new VideoMenu()));
	//setCharacterSize();
}
