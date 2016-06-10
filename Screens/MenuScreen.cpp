#include "MenuScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>
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
	m_game.reset(nullptr);
	m_menu.setKeyboard(false);
}

void	MenuScreen::update(sf::Time frameTime)
{
	AMenu::State				state = m_menu.getState();
	octo::StateManager &		states = octo::Application::getStateManager();
	Progress &					progress = Progress::getInstance();

	if (state == AMenu::State::Hide)
		m_menu.setState(AMenu::State::Active);
	m_game->update(frameTime);

	sf::Vector2f const &		center = octo::Application::getCamera().getCenter();
	sf::Vector2f const &		bubble = m_game->getOctoBubblePosition();
	m_menu.update(frameTime, octo::linearInterpolation(center, bubble, 0.4f));

	if (progress.changeLevel())
	{
		progress.levelChanged();
		states.change("menu");
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
