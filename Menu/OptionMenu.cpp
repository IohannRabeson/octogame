#include "OptionMenu.hpp"
#include "YesNoMenu.hpp"
#include "Progress.hpp"
#include "VideoMenu.hpp"
#include "AudioMenu.hpp"
#include "LanguageMenu.hpp"
#include <StateManager.hpp>
#include <Application.hpp>

class YesNoGroundInfos : public YesNoMenu
{
public:
	void setIndex(void)
	{
//		Progress const & progress = Progress::getInstance();
//		setIndexCursor(progress.isEnableGroundInfos());
	}
	void actionYes(void)
	{
	}
	void actionNo(void)
	{
	}
};


OptionMenu::OptionMenu(void)
{
}

void OptionMenu::createMenus(void)
{
	addMenu(AMenu::getText("options_audio"), std::unique_ptr<AudioMenu>(new AudioMenu()));
	addMenu(AMenu::getText("options_video"), std::unique_ptr<VideoMenu>(new VideoMenu()));
	addMenu(AMenu::getText("options_language"), std::unique_ptr<LanguageMenu>(new LanguageMenu()));
	addMenu(AMenu::getText("options_help_infos"), std::unique_ptr<YesNoGroundInfos>(new YesNoGroundInfos()));

	//setCharacterSize();
}
