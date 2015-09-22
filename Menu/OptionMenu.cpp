#include "OptionMenu.hpp"

OptionMenu::OptionMenu(void)
{
}

void OptionMenu::createMenus(void)
{
	addMenu(L"Audio", std::unique_ptr<AudioMenu>(new AudioMenu()));
	addMenu(L"Vidéo", std::unique_ptr<VideoMenu>(new VideoMenu()));
	//setCharacterSize();
}
