#include "GameScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <Options.hpp>
#include <PostEffectManager.hpp>

GameScreen::GameScreen(void)
{}

void	GameScreen::start()
{
	m_menu.setup();

	octo::GraphicsManager &	graphics = octo::Application::getGraphicsManager();
	Progress &				progress = Progress::getInstance();
	graphics.addKeyboardListener(this);

	progress.load("save.osv");
	m_game.reset(new Game());
	m_game->loadLevel();
}

void	GameScreen::pause()
{
}

void	GameScreen::resume()
{
	Progress::getInstance().levelChanged();
	m_game.reset(new Game());
	m_game->loadLevel();
}

void	GameScreen::stop()
{
	Progress::getInstance().save();
}

void	GameScreen::update(sf::Time frameTime)
{
	AMenu::State state = m_menu.getState();
	octo::StateManager & states = octo::Application::getStateManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();

	if (state == AMenu::State::Active || state == AMenu::State::Draw)
	{
		m_menu.update(frameTime, m_game->getOctoBubblePosition());
		postEffect.setAllShaderEnabled(false);
	}
	else
	{
		postEffect.setAllShaderEnabled(true);
		m_menu.setKeyboard(false);
		m_game->update(frameTime);
		if (Progress::getInstance().changeLevel())
		{
			states.push("transition");
		}
	}
	if (Progress::getInstance().isDead())
	{
		postEffect.setAllShaderEnabled(false);
		Progress::getInstance().setDeath(false);
		states.push("octo_death");
	}
}

bool GameScreen::onPressed(sf::Event::KeyEvent const &event)
{
	switch (event.code)
	{
		case sf::Keyboard::Escape:
			{
				AMenu::State state = m_menu.getState();
				if (state == AMenu::State::Hide)
					m_menu.setState(AMenu::State::Active);
				break;
			}
		case sf::Keyboard::F:
			{
				octo::StateManager & states = octo::Application::getStateManager();
				Progress::getInstance().setNextDestination(Level::Default);
				states.push("transition");
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
