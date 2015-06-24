/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:25:10 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 06:12:46 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "TestBiome.hpp"

#include <Application.hpp>
#include <Camera.hpp>

Game::Game() :
	m_skyDecorManager(500)
{
	m_terrainManager.init(&m_mapManager, &m_biome);
}

void	Game::setup()
{
	m_biomeManager.registerBiome<TestBiome>("test");		
}

void	Game::loadLevel(std::string const& fileName)
{
	(void)fileName;
	// TODO
	m_biomeManager.changeBiome("test", 0x12345);
	m_skyDecorManager.setup(&m_biomeManager.getCurrentBiome());
	m_skyDecorManager.add(DecorManager::DecorTypes::Sun);
}

void	Game::update(sf::Time frameTime)
{
	m_skyDecorManager.update(frameTime, octo::Application::getCamera());
}

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.draw(m_skyDecorManager, states);
}
