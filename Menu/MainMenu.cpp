#include "MainMenu.hpp"
#include "CheatCodeMenu.hpp"
#include "ControlMenu.hpp"
#include "CreditMenu.hpp"
#include "YesNoMenu.hpp"
#include "Progress.hpp"
#include <Camera.hpp>
#include <Application.hpp>

//Sub Menu
class YesNoQuit : public YesNoMenu
{
	inline void setIndex(void) { setIndexCursor(0); }
	inline void actionYes(void) { octo::Application::stop(); }//octo::Application::getStateManager().change("quit"); }
	inline void actionNo(void) { }
};

class YesNoReset : public YesNoMenu
{
	inline void setIndex(void) { setIndexCursor(0); }
	inline void actionYes(void)
	{
		Progress &				progress = Progress::getInstance();
		octo::StateManager &	states = octo::Application::getStateManager();
		progress.reset();
		progress.setFirstTime(true);
		states.change("logo");
	}

	inline void actionNo(void) { }
};

class PlayMenu : public AMenuSelection
{
public:
	PlayMenu(void) = default;
	~PlayMenu(void) = default;
private:
	void createMenus(void)
	{
		addMenu(L"test", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	}
	void onSelection(void)
	{
		setState(AMenu::State::Hide);
		AMenu * backMenu = getBackMenu();
		if (backMenu)
			backMenu->setState(AMenu::State::Active);
		Progress::getInstance().setNextDestination(Level::Rewards);
	}
};

//MainMenu
MainMenu::MainMenu(void)
{
}

void MainMenu::createMenus(void)
{
	if (Progress::getInstance().isMenu())
		addMenu(AMenu::getText("menu_play"), std::unique_ptr<PlayMenu>(new PlayMenu()));
	addMenu(AMenu::getText("menu_controls"), std::unique_ptr<ControlMenu>(new ControlMenu()));
#ifndef NDEBUG
	addMenu(L"Easy", std::unique_ptr<CheatCodeMenu>(new CheatCodeMenu()));
#endif
	addMenu(AMenu::getText("menu_options"), std::unique_ptr<OptionMenu>(new OptionMenu()));
	addMenu(AMenu::getText("menu_credits"), std::unique_ptr<CreditMenu>(new CreditMenu()));
	addMenu(AMenu::getText("menu_restart"), std::unique_ptr<YesNoReset>(new YesNoReset()));
	addMenu(AMenu::getText("menu_quit"), std::unique_ptr<YesNoQuit>(new YesNoQuit()));
	setCharacterSize(30);
	setBubbleType(ABubble::Type::Up);
}

void MainMenu::setup(void)
{
	AMenuSelection::setup();
	m_filter.setSize(octo::Application::getCamera().getSize());
	m_filter.setFillColor(sf::Color(0, 0, 0, 50));
}

void MainMenu::update(sf::Time frameTime, sf::Vector2f const & octoBubblePosition)
{
	AMenuSelection::update(frameTime, octoBubblePosition);
	sf::FloatRect const & camera = octo::Application::getCamera().getRectangle();
	m_filter.setPosition(sf::Vector2f(camera.left, camera.top));
}

void MainMenu::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_filter, states);
	AMenuSelection::draw(render, states);
}
