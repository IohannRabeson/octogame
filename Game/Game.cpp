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
#include "TestBiome.hpp"

#include <Application.hpp>
#include <Camera.hpp>

Game::Game() :
	m_skyDecorManager(2000000)
{
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
	m_skyManager.setup(m_biomeManager.getCurrentBiome(), &m_skyDecorManager);
}

void	Game::update(sf::Time frameTime)
{
	m_skyDecorManager.update(frameTime, octo::Application::getCamera());
	m_skyManager.update(frameTime);
}

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.clear();
	render.draw(m_skyDecorManager, states);
}
