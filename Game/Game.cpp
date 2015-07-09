/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:25:10 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/01 13:28:26 by pciavald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "DefaultBiome.hpp"

#include <Application.hpp>
#include <Camera.hpp>

Game::Game()
{
}

void	Game::setup()
{
	m_biomeManager.registerBiome<DefaultBiome>("test");
}

void	Game::loadLevel(std::string const& fileName)
{
	(void)fileName;
	// TODO
	m_biomeManager.changeBiome("test", 0x12345);

	m_skyManager.setup(m_biomeManager.getCurrentBiome());
	m_groundManager.init(m_biomeManager.getCurrentBiome());
}

void	Game::update(sf::Time frameTime)
{
	m_skyManager.update(frameTime);
	m_groundManager.update(frameTime.asSeconds());
}

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.clear();
	render.draw(m_skyManager.getDecors(), states);
	render.draw(m_groundManager.getDecors(), states);
	render.draw(m_groundManager, states);
}
