#include "DifficultyMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"
#include "TextManager.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

DifficultyMenu::DifficultyMenu(void)
{
}

void DifficultyMenu::createMenus(void)
{
	addMenu(AMenu::getText("options_difficulty_normal"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	if (Progress::getInstance().isFirstTime())
		addMenu(AMenu::getText("options_difficulty_hard"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	setIndexCursor(static_cast<std::size_t>(Progress::getInstance().getDifficulty()));
}

void DifficultyMenu::onSelection(void)
{
	Progress &				progress = Progress::getInstance();
	octo::StateManager &	states = octo::Application::getStateManager();

	progress.setDifficulty(static_cast<Progress::Difficulty>(getIndexCursor()));

	if (progress.isFirstTime())
		states.change("intro");
	else
		states.change("game");

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

