#include "VideoMenu.hpp"
#include "ControlMenu.hpp"

VideoMenu::VideoMenu(void)
{
}

void VideoMenu::createMenus(void)
{
	addMenu("Fullscreen", new ControlMenu());
	addMenu("Vertical Synchronisation", new ControlMenu());
	addMenu("Antialiasing", new ControlMenu());
	addMenu("Resolution", new ControlMenu());
}

