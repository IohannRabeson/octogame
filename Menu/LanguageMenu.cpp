#include "LanguageMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"
#include "TextManager.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

LanguageMenu::LanguageMenu(void)
{
}

void LanguageMenu::createMenus(void)
{
	addMenu(AMenu::getText("options_language_french"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("options_language_english"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	setIndexCursor(static_cast<std::size_t>(Progress::getInstance().getLanguage()));
}

void LanguageMenu::onSelection(void)
{
	Progress & progress = Progress::getInstance();
	progress.setLanguage(static_cast<Progress::Language>(getIndexCursor()));
	TextManager::getInstance().loadTexts();
	if (progress.changeLevel() == false)
	{
		if (progress.isFirstTimeInIceA() && progress.getCurrentDestination() == Level::IceA)
		{
			progress.setNanoRobotCount(0u);
			octo::Application::getStateManager().change("transitionLevelZero");
		}
		else
			octo::Application::getStateManager().change("game");
	}

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

