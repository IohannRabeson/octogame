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
		progress.setFirstTime(false);
		states.change("zero");
	}
	
	inline void onSelection(void)
	{
		if (getIndexCursor() == 0u)
			actionNo();
		else if (getIndexCursor() == 1u)
			actionYes();
	
		setState(AMenu::State::Hide);
	}

	inline void actionNo(void) { }
};

//MainMenu
MainMenu::MainMenu(void) :
	m_nanoCount(0u),
	m_npcCount(0u),
	m_npcMax(0u)
{
}

void MainMenu::createMenus(void)
{
	addMenu(L"Contrôles", std::unique_ptr<ControlMenu>(new ControlMenu()));
#ifndef NDEBUG
	addMenu(L"Easy", std::unique_ptr<CheatCodeMenu>(new CheatCodeMenu()));
#endif
	addMenu(L"Options", std::unique_ptr<OptionMenu>(new OptionMenu()));
	addMenu(L"Crédits", std::unique_ptr<CreditMenu>(new CreditMenu()));
	addMenu(L"Recommencer", std::unique_ptr<YesNoReset>(new YesNoReset()));
	addMenu(L"Quitter", std::unique_ptr<YesNoQuit>(new YesNoQuit()));
	setCharacterSize(30);
	setBubbleType(ABubble::Type::Up);
}

void MainMenu::setup(void)
{
	AMenuSelection::setup();
	m_filter.setSize(octo::Application::getCamera().getSize());
	m_filter.setFillColor(sf::Color(0, 0, 0, 50));
	m_infoText = L"10 / 10 Rencontres";
	m_npcBubble.setup(m_infoText, sf::Color::White);
	m_npcBubble.setType(ABubble::Type::Left);
	m_npcBubble.setActive(true);
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
	m_npcCount = progress.getNpcCount();
	m_npcMax = progress.getNpcMax();
	m_infoText = std::to_wstring(m_npcCount) + L" / " + std::to_wstring(m_npcMax) + L" Rencontres";
	m_npcBubble.setPosition(octoBubblePosition - sf::Vector2f(240.f, 90.f));
	m_npcBubble.setPhrase(m_infoText);
	m_npcBubble.update(frameTime);
	m_infoText = std::to_wstring(m_nanoCount) + L" / 8 Octobots";
	m_infoBubble.setPosition(octoBubblePosition - sf::Vector2f(240.f, 200.f));
	m_infoBubble.setPhrase(m_infoText);
	m_infoBubble.update(frameTime);
}

void MainMenu::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_filter, states);
	AMenuSelection::draw(render, states);
	if (m_nanoCount)
		m_infoBubble.draw(render, states);
	m_npcBubble.draw(render, states);
}
