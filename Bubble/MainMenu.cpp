#include "MainMenu.hpp"
#include "YesNoMenu.hpp"
#include <Camera.hpp>
#include <Application.hpp>

//Sub Menu
class YesNoQuit : public YesNoMenu
{
	inline void setIndex(void) { setIndexCursor(0); }
	inline void actionYes(void) { octo::Application::getStateManager().change("quit"); }
	inline void actionNo(void) { }
};

//MainMenu
MainMenu::MainMenu(void)
{
}

void MainMenu::createMenus(void)
{
	addMenu("Settings", new OptionMenu());
	addMenu("Quit", new YesNoQuit());
	setCharacterSize(40);
	setBubbleType(ABubble::Type::Think);
}

void MainMenu::setup(void)
{
	AMenuSelection::setup();
	m_filter.setSize(octo::Application::getCamera().getSize());
	m_filter.setFillColor(sf::Color(255, 255, 255, 100));
}

void MainMenu::update(sf::Time frameTime, sf::Vector2f const & octoBubblePosition)
{
	AMenuSelection::update(frameTime, octoBubblePosition);
	sf::FloatRect camera = octo::Application::getCamera().getRectangle();
	m_filter.setPosition(sf::Vector2f(camera.left, camera.top));
}

void MainMenu::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_filter, states);
	AMenuSelection::draw(render, states);
}

