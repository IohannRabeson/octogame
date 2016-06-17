#include "OptionMenu.hpp"
#include "YesNoMenu.hpp"
#include "Progress.hpp"
#include "VideoMenu.hpp"
#include "AudioMenu.hpp"
#include "LanguageMenu.hpp"
#include <StateManager.hpp>
#include <Application.hpp>

OptionMenu::OptionMenu(void)
{
}

void OptionMenu::createMenus(void)
{
	addMenu(AMenu::getText("options_audio"), std::unique_ptr<AudioMenu>(new AudioMenu()));
	addMenu(AMenu::getText("options_video"), std::unique_ptr<VideoMenu>(new VideoMenu()));
	addMenu(AMenu::getText("options_language"), std::unique_ptr<LanguageMenu>(new LanguageMenu()));

	//setCharacterSize();
}
