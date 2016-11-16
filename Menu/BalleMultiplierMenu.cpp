#include "BalleMultiplierMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"

#include <Application.hpp>

BalleMultiplierMenu::BalleMultiplierMenu(void)
{
}

void BalleMultiplierMenu::createMenus(void)
{
	for (std::size_t i = 1; i < 12; i++)
		addMenu(std::to_wstring(static_cast<int>(std::round(std::pow(i, 2) / 5.f))), std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	std::size_t nbBalleMultiplier = static_cast<std::size_t>(std::sqrt(Progress::getInstance().getBalleMultiplier() * 5.f));
	setIndexCursor(static_cast<std::size_t>(nbBalleMultiplier));
}

void BalleMultiplierMenu::onSelection(void)
{
	Progress & progress = Progress::getInstance();
	progress.setBalleMultiplier(std::round(std::pow(getIndexCursor(), 2) / 5.f));

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

