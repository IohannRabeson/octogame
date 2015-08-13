/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScreen.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 06:03:08 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 00:26:58 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>

void	GameScreen::start()
{
	m_game.setup();
	m_game.loadLevel("TODO");

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
	m_game.update(frameTime);
}

bool GameScreen::onPressed(sf::Event::KeyEvent const &event)
{
	switch (event.code)
	{
		default:
			break;
	}	
	return (true);
}

void	GameScreen::draw(sf::RenderTarget& render)const
{
	m_game.draw(render, sf::RenderStates());
}
