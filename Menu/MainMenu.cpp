#include "MainMenu.hpp"
#include "CheatCodeMenu.hpp"
#include "ControlMenu.hpp"
#include "CreditMenu.hpp"
#include "YesNoMenu.hpp"
#include "Progress.hpp"
#include "ResourceDefinitions.hpp"
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Application.hpp>

//Sub Menu
class YesNoQuit : public YesNoMenu
{
	inline void setIndex(void) { setIndexCursor(0); }
	inline void actionYes(void)
	{
		Progress &				progress = Progress::getInstance();
		octo::StateManager &	states = octo::Application::getStateManager();

		if (!progress.isMenu())
			states.change("menu");
		else
			octo::Application::stop();
	}
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
		states.change("zero");
	}

	inline void actionNo(void) { }
};

//MainMenu
MainMenu::MainMenu(void) :
	m_soundPlayed(false)
{
}

void MainMenu::createMenus(void)
{
	Progress &				progress = Progress::getInstance();

	if (progress.isMenu())
	{
		addMenu(AMenu::getText("menu_play"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
		addMenu(AMenu::getText("menu_new"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	}
	else
		addMenu(AMenu::getText("menu_controls"), std::unique_ptr<ControlMenu>(new ControlMenu()));
#ifndef NDEBUG
	addMenu(L"Easy", std::unique_ptr<CheatCodeMenu>(new CheatCodeMenu()));
#endif
	if (progress.isGameFinished())
		addMenu(L"Easy", std::unique_ptr<CheatCodeMenu>(new CheatCodeMenu()));
	addMenu(AMenu::getText("menu_options"), std::unique_ptr<OptionMenu>(new OptionMenu()));
	if (progress.isMenu())
	{
		addMenu(AMenu::getText("menu_restart"), std::unique_ptr<YesNoReset>(new YesNoReset()));
		addMenu(AMenu::getText("menu_credits"), std::unique_ptr<CreditMenu>(new CreditMenu()));
		addMenu(AMenu::getText("menu_quit"), std::unique_ptr<YesNoQuit>(new YesNoQuit()));
	}
	else
		addMenu(AMenu::getText("menu_return"), std::unique_ptr<YesNoQuit>(new YesNoQuit()));

	if (progress.isMenu())
		setCharacterSize(50);
	else
		setCharacterSize(30);

	if (progress.isMenu())
		setBubbleType(ABubble::Type::Menu);
	else
		setBubbleType(ABubble::Type::Think);
}

void MainMenu::onSelection(void)
{
	Progress const & progress = Progress::getInstance();

	if (progress.isMenu() && getIndexCursor() == 0u)
	{
		octo::StateManager &	states = octo::Application::getStateManager();

		setState(AMenu::State::Hide);
		AMenu * backMenu = getBackMenu();
		if (backMenu)
			backMenu->setState(AMenu::State::Active);
		if (progress.isFirstTime())
			states.change("zero");
		else
		{
			octo::AudioManager &		audio = octo::Application::getAudioManager();
			octo::ResourceManager &		resources = octo::Application::getResourceManager();
			if (!m_soundPlayed)
			{
				m_soundPlayed = true;
				audio.playSound(resources.getSound(OCTO_GREETING_OGG), 0.7f);
			}
			states.change("transitionLevel");
		}
	}
	else if (progress.isMenu() && getIndexCursor() == 1u && progress.isMenu())
	{
		octo::StateManager &	states = octo::Application::getStateManager();
		states.change("menu");
	}
	else
		AMenuSelection::onSelection();
}

void MainMenu::setup(void)
{
	AMenuSelection::setup();
	m_filter.setSize(octo::Application::getCamera().getSize() * 1.2f);
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
