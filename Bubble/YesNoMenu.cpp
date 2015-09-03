#include "YesNoMenu.hpp"
#include <Application.hpp>

YesNoMenu::YesNoMenu(void)
{
}

void YesNoMenu::createMenus(void)
{
	addMenu("Yes", new EmptyMenu());
	addMenu("No", new EmptyMenu());
	//setCharacterSize();
}

void YesNoMenu::onSelection(void)
{
	if (getIndexCursor() == 0u)
		octo::Application::stop();
	else if (getIndexCursor() == 1u)
	{
		setState(AMenu::State::Hide);
		AMenu * backMenu = getBackMenu();
		if (backMenu)
			backMenu->setState(AMenu::State::Active);
	}
}
