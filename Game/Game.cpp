/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:25:10 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/17 12:07:27 by pciavald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "GenerativeLayer.hpp"

#include <Application.hpp>
#include <Camera.hpp>

Game::Game()
{
}

void	Game::setup()
{
	m_biomeManager.registerBiome<DefaultBiome>("test");

	GenerativeLayer * layer = new GenerativeLayer(sf::Color(185, 185, 30), sf::Vector2f(0.2f, 0.6f));
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x)
			{
				return noise.perlinNoise(x * 10.f, 0.018f, 2, 2.f);
			});
	layer->init();
	layer->setIsScrolling(false);
	m_parallaxScrolling.addLayer(layer);
	layer = new GenerativeLayer(sf::Color(170, 170, 70), sf::Vector2f(0.4f, 0.4f));
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x)
			{
				return noise.perlinNoise(x, 2.018f, 3, 2.f);
			});
	layer->init();
	m_parallaxScrolling.addLayer(layer);
	layer = new GenerativeLayer(sf::Color(180, 180, 110), sf::Vector2f(0.6f, 0.2f));
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x)
			{
				return noise.noise(x * 1.1f, 1.018f);
			});
	layer->init();
	m_parallaxScrolling.addLayer(layer);
}

void	Game::loadLevel(std::string const& fileName)
{
	(void)fileName;
	// TODO
	m_biomeManager.changeBiome("test", 0x12345);

	m_gameClock.setup(m_biomeManager.getCurrentBiome());
	m_skyManager.setup(m_biomeManager.getCurrentBiome(), m_gameClock);
	m_groundManager.init(m_biomeManager.getCurrentBiome());
}

void	Game::update(sf::Time frameTime)
{
	//TODO add update ground
	m_gameClock.update(frameTime);
	m_skyManager.update(frameTime);
	m_groundManager.update(frameTime.asSeconds());
	m_parallaxScrolling.update(frameTime.asSeconds());
}

void	Game::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	render.clear();
	render.draw(m_skyManager.getDecorsBack(), states);
	render.draw(m_parallaxScrolling, states);
	render.draw(m_groundManager.getDecorsBack(), states);
	// Draw Octo and pnj
	render.draw(m_groundManager.getDecorsFront(), states);
	render.draw(m_groundManager, states);
	render.draw(m_skyManager.getDecorsFront(), states);
}
