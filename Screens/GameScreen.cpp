/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScreen.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 06:03:08 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 06:08:01 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameScreen.hpp"
#include "ABiome.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>

void	GameScreen::start()
{
	m_game.setup();
	m_game.loadLevel("TODO");

	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
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
	float cameraSpeed = 500.f * frameTime.asSeconds();
	octo::Camera & camera = octo::Application::getCamera();

	m_game.update(frameTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		camera.move(-cameraSpeed, 0.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		camera.move(cameraSpeed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		camera.move(0.f, -cameraSpeed);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		camera.move(0.f, cameraSpeed);
}

bool GameScreen::onPressed(sf::Event::KeyEvent const &)
{
	return true;
}

void	GameScreen::draw(sf::RenderTarget& render)const
{
	m_game.draw(render, sf::RenderStates());
}
