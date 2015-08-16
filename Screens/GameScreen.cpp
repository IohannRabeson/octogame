/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScreen.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 06:03:08 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 04:30:52 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <AudioManager.hpp>
#include <Options.hpp>

void	GameScreen::start()
{
	m_game.setup();
	m_game.loadLevel("TODO");

	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	octo::AudioManager& audio = octo::Application::getAudioManager();
	octo::Options& option = octo::Application::getOptions();

	graphics.addKeyboardListener(this);
	//TODO: To remove when menu will be implement
	audio.setSoundVolume(option.getValue("sound", 0u));
	audio.setMusicVolume(option.getValue("music", 0u));

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
