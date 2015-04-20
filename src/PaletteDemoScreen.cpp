/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PaletteDemoScreen.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/19 21:23:07 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/20 22:01:59 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PaletteDemoScreen.hpp"
#include <Hsv.hpp>
#include <Application.hpp>
#include <GraphicsManager.hpp>

#include <iostream>

PaletteDemoScreen::PaletteDemoScreen() :
	m_palette(3, 4),
	m_grid(m_palette.getColorCount(), m_palette.getVariationCount() + 1)
{
	static constexpr float const	SquareSize = 64.f;

	m_palette.set(0u, octo::Hsv(0, 1.f, 1.f));
	m_palette.set(1u, octo::Hsv(120, 1.f, 1.f));
	m_palette.set(2u, octo::Hsv(240, 1.f, 1.f));
	m_palette.set(0u, [](octo::Hsv color)
			{
				color.setSaturation(0.5f);
				return (color);
			});
	m_palette.set(1u, [](octo::Hsv color)
			{
				color.setSaturation(0.25f);
				return (color);
			});
	m_palette.set(2u, [](octo::Hsv color)
			{
				color.setValue(0.5f);
				return (color);
			});
	m_palette.set(3u, [](octo::Hsv color)
			{
				color.setHue(color.getHue() + 30);
				return (color);
			});
	for (std::size_t x = 0; x < m_palette.getColorCount(); ++x)
	{
		m_grid(x, 0u).setFillColor(m_palette.get(x));	
	}
	for (std::size_t y = 0u; y < m_grid.rows(); ++y)
	{
		for (std::size_t x = 0u; x < m_grid.columns(); ++x)
		{
			sf::RectangleShape	shape(sf::Vector2f(SquareSize, SquareSize));

			shape.setOrigin(sf::Vector2f(SquareSize * 0.5f, SquareSize * 0.5f));
			shape.setPosition(x * SquareSize, y * SquareSize);
			if (y == 0u)
				shape.setFillColor(m_palette.get(x));
			else
				shape.setFillColor(m_palette.get(x, y - 1u));
			m_grid.set(x, y, shape);
		}
	}
}

void	PaletteDemoScreen::start()
{
	m_view = octo::Application::getGraphicsManager().getDefaultView();
	m_view.setCenter(sf::Vector2f());
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
	for (sf::RectangleShape const& shape : m_grid)
	{
		render.draw(shape);
	}
}
