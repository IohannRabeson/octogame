#include "RandomDiscoverMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"

#include <Application.hpp>

RandomDiscoverMenu::RandomDiscoverMenu(void)
{
}

void RandomDiscoverMenu::createMenus(void)
{
	addMenu(L"0", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"1", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"2", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"3", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"4", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"5", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"6", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"7", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"8", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"9", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"10", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"11", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"12", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"13", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"14", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"15", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"16", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"17", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"18", std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	std::size_t nbRandomDiscover = static_cast<std::size_t>(Progress::getInstance().countRandomDiscover());
	setIndexCursor(static_cast<std::size_t>(nbRandomDiscover));
}

void RandomDiscoverMenu::onSelection(void)
{
	Progress & progress = Progress::getInstance();
	progress.setRandomDiscoverCount(getIndexCursor());
	if (progress.changeLevel() == false)
	{
		progress.setNextDestination(progress.getCurrentDestination());
		progress.setRespawnType(Progress::RespawnType::Portal);
	}

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

