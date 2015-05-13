/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StateGame.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:29:33 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/15 19:31:19 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StateGame.hpp"
#include <Application.hpp>
#include <Camera.hpp>

void	StateGame::start()
{
	octo::Application::getCamera().setZoom(0.75);
	octo::Application::getCamera().move(-310, -200);
	m_mapManager.init(Map::e_map_instance);
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
	render.draw(m_mapManager);
}
