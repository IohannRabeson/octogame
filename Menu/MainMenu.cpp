#include "MainMenu.hpp"
#include "ControlMenu.hpp"
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

//MainMenu
MainMenu::MainMenu(void) :
	m_nanoCount(0u)
{
}

void MainMenu::createMenus(void)
{
	addMenu(L"Contrôles", std::unique_ptr<ControlMenu>(new ControlMenu()));
	addMenu(L"Options", std::unique_ptr<OptionMenu>(new OptionMenu()));
	addMenu(L"Quitter", std::unique_ptr<YesNoQuit>(new YesNoQuit()));
	setCharacterSize(30);
	setBubbleType(ABubble::Type::Up);
}

void MainMenu::setup(void)
{
	AMenuSelection::setup();
	m_filter.setSize(octo::Application::getCamera().getSize());
	m_filter.setFillColor(sf::Color(0, 0, 0, 50));
	m_infoText = L"0 / 8 Octobots";
	m_infoBubble.setup(m_infoText, sf::Color::White);
	m_infoBubble.setType(ABubble::Type::Left);
	m_infoBubble.setActive(true);
}

void MainMenu::update(sf::Time frameTime, sf::Vector2f const & octoBubblePosition)
{
	AMenuSelection::update(frameTime, octoBubblePosition);
	sf::FloatRect const & camera = octo::Application::getCamera().getRectangle();
	Progress & progress = Progress::getInstance();
	m_filter.setPosition(sf::Vector2f(camera.left, camera.top));

	m_nanoCount = progress.getNanoRobotCount();
	m_infoText = std::to_wstring(m_nanoCount) + L" / 8 Octobots";
	m_infoBubble.setPosition(octoBubblePosition - sf::Vector2f(200.f, 90.f));
	m_infoBubble.setPhrase(m_infoText);
	m_infoBubble.update(frameTime);
}

void MainMenu::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_filter, states);
	AMenuSelection::draw(render, states);
	if (m_nanoCount)
		m_infoBubble.draw(render, states);
}
