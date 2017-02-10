#include "ResetMenu.hpp"
#include "YesNoMenu.hpp"
#include "EmptyMenu.hpp"
#include "Progress.hpp"
#include "TextManager.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>

ResetMenu::ResetMenu(void)
{
}

void ResetMenu::createMenus(void)
{
	addMenu(AMenu::getText("options_difficulty_normal"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(AMenu::getText("options_difficulty_hard"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));

	setIndexCursor(static_cast<std::size_t>(Progress::getInstance().getDifficulty()));
}

void ResetMenu::onSelection(void)
{
	Progress &				progress = Progress::getInstance();
	octo::StateManager &	states = octo::Application::getStateManager();

	progress.reset();
	progress.setFirstTime(true);
	progress.setDifficulty(static_cast<Progress::Difficulty>(getIndexCursor()));
	states.change("intro");

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

