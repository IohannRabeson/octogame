#include "VideoMenu.hpp"
#include "EmptyMenu.hpp"
#include "YesNoMenu.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>

//Sub Menu
class YesNoFullscreen : public YesNoMenu
{
public:
	void setIndex(void)
	{
		octo::GraphicsManager const & graphics = octo::Application::getGraphicsManager();
		m_isFullscreen = graphics.isFullscreen();
		setIndexCursor(m_isFullscreen);
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
private:
	bool	m_isFullscreen;
};

class YesNoVsync : public YesNoMenu
{
public:
	void setIndex(void)
	{
		octo::GraphicsManager const & graphics = octo::Application::getGraphicsManager();
		m_isVsync = graphics.isVerticalSyncEnabled();
		setIndexCursor(m_isVsync);
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
private:
	bool	m_isVsync;
};


//Video Menu
VideoMenu::VideoMenu(void)
{
}

void VideoMenu::createMenus(void)
{
	addMenu("Fullscreen", new YesNoFullscreen());
	addMenu("Vertical Synchronisation", new YesNoVsync());
	//TODO:
	//addMenu("Resolution", new EmptyMenu());
}

