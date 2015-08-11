/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScreen.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 06:03:08 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/11 00:10:40 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>

void	GameScreen::start()
{
	m_game.setup();
	m_game.loadLevel("TODO");

	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	octo::ResourceManager& resources = octo::Application::getResourceManager();
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();

	graphics.addKeyboardListener(this);
	m_shader0.loadFromMemory(resources.getText(PIXELATE_FRAG), sf::Shader::Fragment);
	m_shader0.setParameter("pixel_threshold", 0.005f);
	m_shader1.loadFromMemory(resources.getText(GRAY_FRAG), sf::Shader::Fragment);
	postEffect.addShader(m_shader0, false);
	postEffect.addShader(m_shader1, false);
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
	octo::PostEffectManager& postEffect = octo::Application::getPostEffectManager();

	switch (event.code)
	{
		case sf::Keyboard::F1:
			postEffect.enableShader(0, !postEffect.isShaderEnabled(0));
			break;
		case sf::Keyboard::F2:
			postEffect.enableShader(1, !postEffect.isShaderEnabled(1));
			break;
		default:
			break;
	}	
	return (true);
}

void	GameScreen::draw(sf::RenderTarget& render)const
{
	m_game.draw(render, sf::RenderStates());
}
