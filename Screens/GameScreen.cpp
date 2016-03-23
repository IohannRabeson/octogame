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
	m_game.reset(new Game());
	m_game->loadLevel();
	m_menu.setup();
}

void	GameScreen::pause()
{
	InputListener::removeInputListener();
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
}

void	GameScreen::update(sf::Time frameTime)
{
	AMenu::State				state = m_menu.getState();
	octo::StateManager &		states = octo::Application::getStateManager();
	octo::PostEffectManager &	postEffect = octo::Application::getPostEffectManager();
	Progress &					progress = Progress::getInstance();

	if (state == AMenu::State::Active || state == AMenu::State::Draw)
	{
		m_menu.update(frameTime, m_game->getOctoBubblePosition());
		postEffect.setAllShaderEnabled(false);
		m_doSave = true;
	}
	else if (m_doSave)
	{
		progress.save();
		m_doSave = false;
	}
	else
	{
		postEffect.setAllShaderEnabled(true);
		m_menu.setKeyboard(false);
		m_game->update(frameTime);
		if (progress.changeLevel())
		{
			progress.save();
			progress.levelChanged();
			states.change("transitionLevel");
		}
	}
}

bool GameScreen::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Escape:
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
