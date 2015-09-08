/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitScreen.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/30 01:22:07 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/15 14:39:39 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QuitScreen.hpp"
#include <StateManager.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

#include "ResourceDefinitions.hpp"

QuitScreen::QuitScreen() :
	AbstractState()
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);

	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	octo::Application::getGraphicsManager().setIcon(resources.getTexture(ICON_PNG).copyToImage());
	m_startTexture = resources.getTexture(START_SCREEN_PNG);
	m_startSprite.setTexture(m_startTexture);
	m_startSprite.setOrigin(m_startSprite.getLocalBounds().width / 2.f, m_startSprite.getLocalBounds().height / 2.f);
	m_startSprite.setPosition(octo::Application::getCamera().getCenter());
	m_startSprite.setScale(1.2f, 1.2f);
}

void	QuitScreen::start()
{
}

void	QuitScreen::pause()
{
}

void	QuitScreen::resume()
{
}

void	QuitScreen::stop()
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(this);
	octo::Application::stop();
}

void	QuitScreen::update(sf::Time frameTime)
{
	m_timer += frameTime;
	if (m_timer > sf::seconds(2.f))
		stop();
}

void	QuitScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	render.draw(m_startSprite);
}

bool	QuitScreen::onPressed(sf::Event::KeyEvent const &event)
{
	switch (event.code)
	{
		case sf::Keyboard::Escape:
		{
			stop();
			break;
		}
		default:
			break;
	}	
	return (true);
}
