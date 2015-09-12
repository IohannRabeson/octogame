#include "GameScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <Options.hpp>
#include <PostEffectManager.hpp>

GameScreen::GameScreen(void) :
	m_changeLevel(true)
{}

void	GameScreen::start()
{
	m_menu.setup();

	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);
}

void	GameScreen::pause()
{
}

void	GameScreen::resume()
{
}

void	GameScreen::stop()
{
}

void	GameScreen::update(sf::Time frameTime)
{
	if (m_changeLevel)
	{
		m_game.reset(new Game());
		m_game->setup();
		m_game->loadLevel("default");
		m_changeLevel = false;
		return;
	}

	AMenu::State state = m_menu.getState();
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
			m_changeLevel = true;
			octo::StateManager & states = octo::Application::getStateManager();
			states.push("game", "default");
			break;
		}
		default:
			break;
	}
	return (true);
}

void	GameScreen::draw(sf::RenderTarget& render)const
{
	if (m_changeLevel)
		return;
	m_game->draw(render, sf::RenderStates());
	if (m_menu.getState() == AMenu::State::Active || m_menu.getState() == AMenu::State::Draw)
		render.draw(m_menu);
}
