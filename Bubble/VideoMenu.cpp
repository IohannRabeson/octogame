#include "VideoMenu.hpp"
#include "YesNoMenu.hpp"

VideoMenu::VideoMenu(void)
{
}

void VideoMenu::createMenus(void)
{
	addMenu("Fullscreen", new YesNoMenu());
	addMenu("Vertical Synchronisation", new YesNoMenu());
	addMenu("Antialiasing", new YesNoMenu());
	addMenu("Resolution", new YesNoMenu());
}

