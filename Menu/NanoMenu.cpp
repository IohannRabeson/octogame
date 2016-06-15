#include "NanoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"

#include <Application.hpp>

NanoMenu::NanoMenu(void)
{
}

void NanoMenu::createMenus(void)
{
	addMenu(L"0", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"1", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"2", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"3", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"4", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"5", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"6", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"7", std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	std::size_t nbNano = static_cast<std::size_t>(Progress::getInstance().getNanoRobotCount());
	if (nbNano >= 7u)
		nbNano = 7u;
	setIndexCursor(static_cast<std::size_t>(nbNano));
}

void NanoMenu::onSelection(void)
{
	Progress & progress = Progress::getInstance();
	progress.setNanoRobotCount(getIndexCursor());
	if (progress.changeLevel() == false)
	{
		progress.setNextDestination(progress.getLastDestination());
		progress.setRespawnType(Progress::RespawnType::Portal);
	}

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

