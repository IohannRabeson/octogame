#include "YesNoMenu.hpp"
#include <Application.hpp>

YesNoMenu::YesNoMenu(void)
{
}

void YesNoMenu::createMenus(void)
{
	addMenu(L"No", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"Yes", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	setIndex();
	//setCharacterSize();
}

void YesNoMenu::onSelection(void)
{
	if (getIndexCursor() == 0u)
		actionNo();
	else if (getIndexCursor() == 1u)
		actionYes();

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

