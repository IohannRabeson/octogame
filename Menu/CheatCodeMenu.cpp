#include "CheatCodeMenu.hpp"
#include "LevelMenu.hpp"
#include "NanoMenu.hpp"
#include "RandomDiscoverMenu.hpp"
#include "Progress.hpp"
#include "YesNoMenu.hpp"
#include "SteamAPI.hpp"

class YesNoResetSteam : public YesNoMenu
{
public:
	void setIndex(void)
	{
		setIndexCursor(1u);
	}
	void actionYes(void)
	{
		Progress::getSteamInstance().reset();
	}
	void actionNo(void)
	{
	}
};

CheatCodeMenu::CheatCodeMenu(void)
{
}

void CheatCodeMenu::createMenus(void)
{
	addMenu(L"Biomes", std::unique_ptr<LevelMenu>(new LevelMenu()));
	addMenu(L"Octobots", std::unique_ptr<NanoMenu>(new NanoMenu()));
	addMenu(L"Random", std::unique_ptr<RandomDiscoverMenu>(new RandomDiscoverMenu()));
	addMenu(L"Reset Steam", std::unique_ptr<YesNoResetSteam>(new YesNoResetSteam()));
}

