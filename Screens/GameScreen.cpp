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
#include <Camera.hpp>

void	GameScreen::start()
{
	m_game.setup();
	m_game.loadLevel("TODO");

	m_isMenu = false;
	m_menu.setup("test", sf::Color(255, 255, 255, 255), 50u);
	m_menu.setType(ABubble::Type::None);
	m_filter.setSize(octo::Application::getCamera().getSize());
	m_filter.setFillColor(sf::Color(255, 255, 255, 100));

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
	if (m_isMenu)
	{
		m_menu.setType(ABubble::Type::Think);
		m_menu.setPosition(m_game.getOctoBubblePosition());
		m_menu.update(frameTime);
		sf::FloatRect camera = octo::Application::getCamera().getRectangle();
		m_filter.setPosition(sf::Vector2f(camera.left, camera.top));
	}
	else
	{
		m_menu.setType(ABubble::Type::None);
		m_game.update(frameTime);
	}
}

bool GameScreen::onPressed(sf::Event::KeyEvent const &event)
{
	switch (event.code)
	{
		case sf::Keyboard::Return:
		{
			if (m_isMenu == false)
				m_isMenu = true;
			else if (m_isMenu == true)
				m_isMenu = false;
			break;
		}
		default:
			break;
	}	
	return (true);
}

void	GameScreen::draw(sf::RenderTarget& render)const
{
	m_game.draw(render, sf::RenderStates());
	if (m_isMenu)
	{
		render.draw(m_filter);
		render.draw(m_menu);
	}
}
