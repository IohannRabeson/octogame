#include "VideoMenu.hpp"
#include "EmptyMenu.hpp"
#include "YesNoMenu.hpp"
#include "ResolutionMenu.hpp"
#include "BalleMultiplierMenu.hpp"
#include "Progress.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>

//Sub Menu
class YesNoFullscreen : public YesNoMenu
{
public:
	void setIndex(void)
	{
		octo::GraphicsManager const & graphics = octo::Application::getGraphicsManager();
		setIndexCursor(graphics.isFullscreen());
	}
	void actionYes(void)
	{
		octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
		graphics.setFullscreen(true);
	}
	void actionNo(void)
	{
		octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
		graphics.setFullscreen(false);
	}
};

class YesNoVsync : public YesNoMenu
{
public:
	void setIndex(void)
	{
		octo::GraphicsManager const & graphics = octo::Application::getGraphicsManager();
		setIndexCursor(graphics.isVerticalSyncEnabled());
	}
	void actionYes(void)
	{
		octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
		graphics.setVerticalSyncEnabled(true);
	}
	void actionNo(void)
	{
		octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
		graphics.setVerticalSyncEnabled(false);
	}
};

class DetailsMenu : public AMenuSelection
{
	void createMenus(void)
	{
		Progress & progress = Progress::getInstance();

		addMenu(L"Low", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
		addMenu(L"Normal", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
		addMenu(L"High", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
		addMenu(L"Ultra", std::unique_ptr<EmptyMenu>(new EmptyMenu()));

		setIndexCursor(progress.getLevelOfDetails() + 2);
	}

	void onSelection(void)
	{
		Progress &				progress = Progress::getInstance();

		if (getIndexCursor() == 0u)
			progress.setLevelOfDetails(-2);
		if (getIndexCursor() == 1u)
			progress.setLevelOfDetails(-1);
		if (getIndexCursor() == 2u)
			progress.setLevelOfDetails(0);
		if (getIndexCursor() == 3u)
			progress.setLevelOfDetails(1);

		setState(AMenu::State::Hide);
		AMenu * backMenu = getBackMenu();
		if (backMenu)
			backMenu->setState(AMenu::State::Active);
	}
};

class FramerateMenu : public AMenuSelection
{
	void createMenus(void)
	{
		Progress & progress = Progress::getInstance();

		addMenu(L"30", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
		addMenu(L"60", std::unique_ptr<EmptyMenu>(new EmptyMenu()));

		setIndexCursor(progress.getLevelOfDetails() + 2);
	}

	void onSelection(void)
	{
		octo::GraphicsManager &	graphics = octo::Application::getGraphicsManager();

		if (getIndexCursor() == 0u)
			graphics.setFramerateLimit(30);
		if (getIndexCursor() == 1u)
			graphics.setFramerateLimit(62);

		setState(AMenu::State::Hide);
		AMenu * backMenu = getBackMenu();
		if (backMenu)
			backMenu->setState(AMenu::State::Active);
	}
};

//Video Menu
VideoMenu::VideoMenu(void)
{
}

void VideoMenu::createMenus(void)
{
	Progress & progress = Progress::getInstance();

	addMenu(AMenu::getText("options_video_fullscreen"), std::unique_ptr<YesNoFullscreen>(new YesNoFullscreen()));
	addMenu(AMenu::getText("options_video_vsync"), std::unique_ptr<YesNoVsync>(new YesNoVsync()));
	addMenu(AMenu::getText("options_video_details"), std::unique_ptr<DetailsMenu>(new DetailsMenu()));
	addMenu(AMenu::getText("options_video_framerate"), std::unique_ptr<FramerateMenu>(new FramerateMenu()));
	if (progress.isGameFinished())
		addMenu(L"???", std::unique_ptr<BalleMultiplierMenu>(new BalleMultiplierMenu()));
	//TODO: Make resolution work
	//addMenu("Resolution", std::unique_ptr<ResolutionMenu>(new ResolutionMenu()));
}

