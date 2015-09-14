#include "ControlMenu.hpp"

ControlMenu::ControlMenu(void)
{
}

void ControlMenu::createMenus(void)
{
	addMenu("Left/Right : move", nullptr);
	addMenu("Space : jump, double jump", nullptr);
	addMenu("Up : slow fall, climb elevators", nullptr);
}
