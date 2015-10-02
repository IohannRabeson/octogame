#include "OptionMenu.hpp"
#include "YesNoMenu.hpp"
#include "Progress.hpp"
#include <StateManager.hpp>
#include <Application.hpp>

class YesNoReset : public YesNoMenu
{
	inline void setIndex(void) { setIndexCursor(0); }
	inline void actionYes(void)
	{
		Progress &				progress = Progress::getInstance();
		octo::StateManager &	states = octo::Application::getStateManager();
		progress.reset();
		progress.setFirstTime(false);
		states.change("zero");
	}
	inline void actionNo(void) { }
};


OptionMenu::OptionMenu(void)
{
}

void OptionMenu::createMenus(void)
{
	addMenu(L"Audio", std::unique_ptr<AudioMenu>(new AudioMenu()));
	addMenu(L"Vidéo", std::unique_ptr<VideoMenu>(new VideoMenu()));
	addMenu(L"Recommencer", std::unique_ptr<YesNoReset>(new YesNoReset()));
	//setCharacterSize();
}
