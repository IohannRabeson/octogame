#include "TransitionScreen.hpp"
#include <Application.hpp>
#include <Camera.hpp>

void	TransitionScreen::start()
{
	m_timer = sf::Time::Zero;
	m_timerMax = sf::seconds(0.f);
}

void	TransitionScreen::pause()
{
}

void	TransitionScreen::resume()
{
}

void	TransitionScreen::stop()
{
}

void	TransitionScreen::update(sf::Time frameTime)
{
	m_timer += frameTime;
	if (m_timer >= m_timerMax)
	{
		octo::StateManager & states = octo::Application::getStateManager();
		states.pop();
	}
}

void	TransitionScreen::draw(sf::RenderTarget & render) const
{
	render.clear(sf::Color::Black);
}
