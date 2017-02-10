#include "MenuTypeMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"
#include "TextManager.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

MenuTypeMenu::MenuTypeMenu(void)
{
}

void MenuTypeMenu::createMenus(void)
{
	addMenu(AMenu::getText("options_menutype_classic"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("options_menutype_simple"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	setIndexCursor(static_cast<std::size_t>(Progress::getInstance().getMenuType()));
}

void MenuTypeMenu::onSelection(void)
{
	Progress & progress = Progress::getInstance();
	progress.setMenuType(static_cast<Progress::MenuType>(getIndexCursor()));
	TextManager::getInstance().loadTexts();

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

