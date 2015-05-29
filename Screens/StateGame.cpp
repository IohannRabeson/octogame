/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StateGame.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:29:33 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/29 11:59:48 by pciavald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StateGame.hpp"
#include "../ResourceDefinitions.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <ResourceManager.hpp>

void	StateGame::start()
{
	//octo::Application::getCamera().setZoom(0.75);
	//octo::Application::getCamera().move(-310, -200);
	m_mapManager.init();

	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	m_back.setSize(sf::Vector2f(2000, 1300));
	m_back.setTexture(&resources.getTexture(BACKGROUND_PNG));
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

void	StateGame::update(sf::Time p_deltatime)
{
	m_mapManager.update(p_deltatime.asSeconds());
}

void	StateGame::draw(sf::RenderTarget& render) const
{
	render.clear(sf::Color::Black);
	render.draw(m_back);
	render.draw(m_mapManager);
}
