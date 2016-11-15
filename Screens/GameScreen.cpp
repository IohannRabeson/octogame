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
	m_doSave(false),
	m_timerRedBlueMax(sf::seconds(30.f))
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
	m_timePlayed.restart();
}

void	GameScreen::pause()
{
}

void	GameScreen::resume()
{
}

void	GameScreen::stop()
{
	octo::Application::getAudioManager().stopMusic(sf::Time::Zero);
	Progress::getInstance().save(m_timePlayed.getElapsedTime().asSeconds());
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
		changeLevel(states, progress);
	}
	timeLevelBlueRed(frameTime, states, progress);
}

void GameScreen::changeLevel(octo::StateManager & states, Progress & progress)
{
	if (progress.changeLevel())
	{
		Level current = progress.getCurrentDestination();
		Level next = progress.getNextDestination();

		progress.levelChanged();
		if (current == Level::Blue || next == Level::Blue)
		{
			if (next == Level::Blue)
				states.setTransitionDuration(sf::seconds(2.5f), sf::seconds(0.0f));
			states.change("transitionLevel", "blue");
		}
		else if (current == Level::Red || next == Level::Red)
		{
			if (next == Level::Red)
				states.setTransitionDuration(sf::seconds(2.5f), sf::seconds(0.f));
			states.change("transitionLevel", "red");
		}
		else
		{
			states.setTransitionDuration(sf::seconds(0.5f), sf::seconds(0.5f));
			states.change("transitionLevel");
		}
	}
}

void GameScreen::timeLevelBlueRed(sf::Time frameTime, octo::StateManager & states, Progress & progress)
{
	Level next = progress.getNextDestination();

	if (next == Level::Blue)
	{
		m_timerBlue += frameTime;
		if (m_timerBlue >= m_timerRedBlueMax)
		{
			progress.setNextDestination(Level::IceA);
			states.setTransitionDuration(sf::seconds(2.5f), sf::seconds(2.0f));
		}
	}
	else if (next == Level::Red)
	{
		m_timerRed += frameTime;
		if (m_timerRed >= m_timerRedBlueMax)
		{
			states.setTransitionDuration(sf::seconds(2.5f), sf::seconds(2.0f));
			states.change("laboratory_end", "red");
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
