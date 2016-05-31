#include "MenuScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <Options.hpp>
#include <PostEffectManager.hpp>

MenuScreen::MenuScreen(void)
{}

void	MenuScreen::start()
{
	Progress &				progress = Progress::getInstance();

	InputListener::addInputListener();

	progress.load("save.osv");
	progress.setMenu(true);
	m_game.reset(new Game());
	m_game->loadLevel();
	m_menu.setup();
}

void	MenuScreen::pause()
{
	InputListener::removeInputListener();
	m_menu.setKeyboard(false);
}

void	MenuScreen::resume()
{
	octo::Application::getPostEffectManager().removeEffects();
	Progress::getInstance().levelChanged();

	InputListener::addInputListener();
	m_game.reset(new Game());
	m_game->loadLevel();
}

void	MenuScreen::stop()
{
	octo::Application::getAudioManager().stopMusic(sf::Time::Zero);
	Progress::getInstance().save();
	octo::Application::getPostEffectManager().removeEffects();
	InputListener::removeInputListener();
	m_menu.setKeyboard(false);
}

void	MenuScreen::update(sf::Time frameTime)
{
	AMenu::State				state = m_menu.getState();
	octo::StateManager &		states = octo::Application::getStateManager();
	Progress &					progress = Progress::getInstance();

	if (state == AMenu::State::Hide)
		m_menu.setState(AMenu::State::Active);
	m_menu.update(frameTime, m_game->getOctoBubblePosition());
	m_game->update(frameTime);
	if (progress.changeLevel())
	{
		if (progress.isFirstTime())
		{
			progress.save();
			states.change("zero");
		}
		else
		{
			progress.levelChanged();
			states.change("transitionLevel");
		}
	}
}

bool MenuScreen::onInputPressed(InputListener::OctoKeys const & key)
{
	(void)key;
	return true;
}

void	MenuScreen::draw(sf::RenderTarget& render)const
{
	m_game->draw(render, sf::RenderStates());
	if (m_menu.getState() == AMenu::State::Active || m_menu.getState() == AMenu::State::Draw)
		render.draw(m_menu);
}
