#include "YesNoMenu.hpp"
#include <Application.hpp>

YesNoMenu::YesNoMenu(void)
{
}

void YesNoMenu::createMenus(void)
{
	addMenu("No", new EmptyMenu());
	addMenu("Yes", new EmptyMenu());
	//setCharacterSize();
}

void YesNoMenu::onSelection(void)
{
	if (getIndexCursor() == 0u)
	{
		setState(AMenu::State::Hide);
		AMenu * backMenu = getBackMenu();
		if (backMenu)
			backMenu->setState(AMenu::State::Active);
	}
	else if (getIndexCursor() == 1u)
		action();
}

void YesNoMenu::setIndex(std::size_t index)
{
	setIndexCursor(index);
}

void YesNoMenu::action(void)
{
	octo::Application::stop();
}
