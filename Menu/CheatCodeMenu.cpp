#include "CheatCodeMenu.hpp"
#include "LevelMenu.hpp"
#include "NanoMenu.hpp"
#include "Progress.hpp"

CheatCodeMenu::CheatCodeMenu(void)
{
}

void CheatCodeMenu::createMenus(void)
{
	addMenu(L"Biomes", std::unique_ptr<LevelMenu>(new LevelMenu()));
	addMenu(L"Octobots", std::unique_ptr<NanoMenu>(new NanoMenu()));
}

