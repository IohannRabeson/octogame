/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StateGame.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:29:33 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/01 16:17:59 by pciavald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StateGame.hpp"
#include "../ResourceDefinitions.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <ResourceManager.hpp>

void	StateGame::start()
{
	m_mapManager.init();

	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	m_back.setSize(sf::Vector2f(1900, 1080));
	m_back.setTexture(&resources.getTexture(BACKGROUND_PNG));
	//m_back.setFillColor(sf::Color(255,255,255, 20));
}

void	StateGame::pause()
{
}

void	StateGame::resume()
{
}

void	StateGame::stop()
{
}
#include <unistd.h>
void	StateGame::update(sf::Time p_deltatime)
{
	m_mapManager.update(p_deltatime.asSeconds());

	/*
	static int i = 0;
	if (i++ < 30)
	{
		m_back.setFillColor(sf::Color(255,255,255, i));
	}
	else if (i < 60)
	{
		m_back.setFillColor(sf::Color(255,255,255, 30 - i / 2));
	}
	else
		i = 0;
	*/
}

void	StateGame::draw(sf::RenderTarget& render) const
{
	render.clear(sf::Color::Black);
	//render.draw(m_back);
	render.draw(m_mapManager);
}
