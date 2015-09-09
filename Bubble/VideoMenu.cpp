#include "VideoMenu.hpp"
#include "EmptyMenu.hpp"
#include "YesNoMenu.hpp"
#include "ResolutionMenu.hpp"

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


//Video Menu
VideoMenu::VideoMenu(void)
{
}

void VideoMenu::createMenus(void)
{
	addMenu("Fullscreen", std::unique_ptr<YesNoFullscreen>(new YesNoFullscreen()));
	addMenu("VSync", std::unique_ptr<YesNoVsync>(new YesNoVsync()));
	//TODO: Make resolution work
	//addMenu("Resolution", std::unique_ptr<ResolutionMenu>(new ResolutionMenu()));
}

