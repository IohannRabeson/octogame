#include "GameScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <Options.hpp>
#include <PostEffectManager.hpp>

GameScreen::GameScreen(void) :
	m_doSave(false)
{}

void	GameScreen::start()
{
	Progress &				progress = Progress::getInstance();

	InputListener::addInputListener();
	progress.load("save.osv");
	progress.setMenu(false);
	m_game.reset(new Game());
	m_game->loadLevel();
	m_menu.setup();
}

void	GameScreen::pause()
{
	InputListener::removeInputListener();
	m_menu.setKeyboard(false);
}

void	GameScreen::resume()
{
	octo::Application::getPostEffectManager().removeEffects();
	Progress::getInstance().levelChanged();

	InputListener::addInputListener();
	m_game.reset(new Game());
	m_game->loadLevel();
}

void	GameScreen::stop()
{
	octo::Application::getAudioManager().stopMusic(sf::Time::Zero);
	Progress::getInstance().save();
	octo::Application::getPostEffectManager().removeEffects();
	InputListener::removeInputListener();
	m_game.reset(nullptr);
	m_menu.setKeyboard(false);
}

void	GameScreen::update(sf::Time frameTime)
{
	AMenu::State				state = m_menu.getState();
	octo::StateManager &		states = octo::Application::getStateManager();
	Progress &					progress = Progress::getInstance();

	m_menu.update(frameTime, m_game->getOctoBubblePosition());
	if (state == AMenu::State::Active || state == AMenu::State::Draw)
	{
		progress.setBubbleNpc(false);
		m_doSave = true;
	}
	else if (m_doSave)
	{
		progress.setBubbleNpc(true);
		progress.save();
		m_doSave = false;
	}
	else
	{
		m_menu.setKeyboard(false);
		m_game->update(frameTime);
		if (progress.changeLevel())
		{
			progress.levelChanged();
			states.change("transitionLevel");
		}
	}
}

bool GameScreen::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Menu:
			{
				AMenu::State state = m_menu.getState();
				if (state == AMenu::State::Hide)
					m_menu.setState(AMenu::State::Active);
				break;
			}
		default:
			break;
	}
	return (true);
}

void	GameScreen::draw(sf::RenderTarget& render)const
{
	m_game->draw(render, sf::RenderStates());
	if (m_menu.getState() == AMenu::State::Active || m_menu.getState() == AMenu::State::Draw)
		render.draw(m_menu);
}
