/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PaletteDemoScreen.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/19 21:23:07 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/26 21:23:38 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PaletteDemoScreen.hpp"
#include "../ResourceDefinitions.hpp"

#include <Hsv.hpp>
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>

#include <iostream>

PaletteDemoScreen::PaletteDemoScreen()
{
}

void	PaletteDemoScreen::start()
{
	sf::Vector2f		pos(-200, -200);
	sf::RectangleShape	rect(sf::Vector2f(32, 32));

	rect.setOrigin(sf::Vector2f(16, 16));
	m_view = octo::Application::getGraphicsManager().getDefaultView();
	m_view.setCenter(sf::Vector2f());
	m_palette = &octo::Application::getResourceManager().getPalette(PALETTE_DEMO_OPA);
	for (std::size_t i = 0; i < m_palette->getColorCount(); ++i)
	{
		if (pos.x > 200.f)
		{
			pos.x = -200.f;
			pos.y += 32.f;
		}
		rect.setPosition(pos);
		rect.setFillColor(m_palette->getColor(i));
		rect.setOutlineThickness(1.f);
		rect.setOutlineColor(sf::Color::White);
		m_rectangles.push_back(rect);
		pos.x += 32.f;
	}
}

void	PaletteDemoScreen::pause()
{
}

void	PaletteDemoScreen::resume()
{
}

void	PaletteDemoScreen::stop()
{
}

void	PaletteDemoScreen::update(sf::Time)
{
}

void	PaletteDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	render.setView(m_view);
	for (sf::RectangleShape const& shape : m_rectangles)
	{
		render.draw(shape);
	}
}
