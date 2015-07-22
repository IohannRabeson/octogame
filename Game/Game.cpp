/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:25:10 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/22 16:12:53 by pciavald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "DefaultBiome.hpp"
#include "GenerativeLayer.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

Game::Game()
{
}

void	Game::setup()
{
	m_biomeManager.registerBiome<DefaultBiome>("test");
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);
}

void	Game::loadLevel(std::string const& fileName)
{
	(void)fileName;
	// TODO
	m_biomeManager.changeBiome("test", 0x12345);

	m_gameClock.setup(m_biomeManager.getCurrentBiome());
	m_skyManager.setup(m_biomeManager.getCurrentBiome(), m_gameClock);
	m_groundManager.init(m_biomeManager.getCurrentBiome());

	sf::Vector2u const & mapSize = m_biomeManager.getCurrentBiome().getMapSize();
	GenerativeLayer * layer = new GenerativeLayer(sf::Color(185, 185, 30), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -0, 0.4f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
			{
				return noise.perlinNoise(x * 10.f, y, 2, 2.f);
			});
	m_parallaxScrolling.addLayer(layer);
	layer = new GenerativeLayer(sf::Color(170, 170, 70), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -10, 0.3f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
			{
				return noise.perlinNoise(x, y, 3, 2.f);
			});
	m_parallaxScrolling.addLayer(layer);
	layer = new GenerativeLayer(sf::Color(180, 180, 110), sf::Vector2f(0.6f, 0.2f), mapSize, 12.f, -10, 0.2f, 6.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
			{
				return noise.noise(x * 1.1f, y);
			});
	m_parallaxScrolling.addLayer(layer);
}

void	Game::update(sf::Time frameTime)
{
	m_gameClock.update(frameTime);
	m_skyManager.update(frameTime);
	m_groundManager.update(frameTime.asSeconds());
	m_parallaxScrolling.update(frameTime.asSeconds());
}

bool Game::onPressed(sf::Event::KeyEvent const & event)
{
	switch (event.code)
	{
		case sf::Keyboard::E:
			m_groundManager.setNextGenerationState(GroundManager::GenerationState::Next);
		break;
		case sf::Keyboard::R:
			m_groundManager.setNextGenerationState(GroundManager::GenerationState::Previous);
		break;
		default:
		break;
	}
	return true;
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
	render.draw(m_groundManager.getDecorsGround(), states);
	render.draw(m_skyManager.getDecorsFront(), states);
}
