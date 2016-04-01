#include "LanguageMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

LanguageMenu::LanguageMenu(void) :
	m_lastLanguage(0u)
{
}

void LanguageMenu::createMenus(void)
{
	AMenu::initTexts();
	addMenu(AMenu::getText("options_language_french"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("options_language_english"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	setIndexCursor(static_cast<std::size_t>(Progress::getInstance().getLanguage()));
	m_lastLanguage = static_cast<std::size_t>(Progress::getInstance().getLanguage());
}

void LanguageMenu::onSelection(void)
{
	if (getIndexCursor() != m_lastLanguage)
	{
		Progress & progress = Progress::getInstance();
		progress.setLanguage(static_cast<Progress::Language>(getIndexCursor()));
		if (progress.changeLevel() == false)
			progress.setNextDestination(progress.getLastDestination());
	}

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

