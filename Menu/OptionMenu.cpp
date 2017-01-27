#include "OptionMenu.hpp"
#include "YesNoMenu.hpp"
#include "Progress.hpp"
#include "VideoMenu.hpp"
#include "AudioMenu.hpp"
#include "DifficultyMenu.hpp"
#include "BalleMultiplierMenu.hpp"
#include "LanguageMenu.hpp"
#include "MenuTypeMenu.hpp"
#include "CheatCodeMenu.hpp"
#include <StateManager.hpp>
#include <Application.hpp>

class YesNoRestartLevel : public YesNoMenu
{
	inline void setIndex(void) { setIndexCursor(0); }
	inline void actionYes(void)
	{
		Progress &				progress = Progress::getInstance();
		octo::StateManager &	states = octo::Application::getStateManager();

		progress.setNextDestination(progress.getCurrentDestination());
		states.change("game");
	}
	inline void actionNo(void) { }
};

OptionMenu::OptionMenu(void)
{
}

void OptionMenu::createMenus(void)
{
	Progress &				progress = Progress::getInstance();

	#ifndef NDEBUG
	addMenu(L"Easy", std::unique_ptr<CheatCodeMenu>(new CheatCodeMenu()));
	#endif
	if (progress.isGameFinished())
		addMenu(L"???", std::unique_ptr<BalleMultiplierMenu>(new BalleMultiplierMenu()));
	addMenu(AMenu::getText("options_audio"), std::unique_ptr<AudioMenu>(new AudioMenu()));
	addMenu(AMenu::getText("options_video"), std::unique_ptr<VideoMenu>(new VideoMenu()));
	if (progress.getDifficulty() == Progress::Difficulty::Hard)
		addMenu(AMenu::getText("options_difficulty"), std::unique_ptr<DifficultyMenu>(new DifficultyMenu()));
	addMenu(AMenu::getText("options_language"), std::unique_ptr<LanguageMenu>(new LanguageMenu()));
	addMenu(AMenu::getText("options_menutype"), std::unique_ptr<MenuTypeMenu>(new MenuTypeMenu()));
	if (!progress.isMenu())
		addMenu(AMenu::getText("options_unlock"), std::unique_ptr<YesNoRestartLevel>(new YesNoRestartLevel()));

	//setCharacterSize();
}
