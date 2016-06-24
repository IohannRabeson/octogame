#include "CheatCodeMenu.hpp"
#include "LevelMenu.hpp"
#include "NanoMenu.hpp"
#include "RandomDiscoverMenu.hpp"
#include "Progress.hpp"

CheatCodeMenu::CheatCodeMenu(void)
{
}

void CheatCodeMenu::createMenus(void)
{
	addMenu(L"Biomes", std::unique_ptr<LevelMenu>(new LevelMenu()));
	addMenu(L"Octobots", std::unique_ptr<NanoMenu>(new NanoMenu()));
	addMenu(L"Random", std::unique_ptr<RandomDiscoverMenu>(new RandomDiscoverMenu()));
}

