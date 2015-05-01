/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PaletteDemoScreen.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/19 21:23:07 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/01 15:49:54 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PaletteDemoScreen.hpp"
#include "../ResourceDefinitions.hpp"

#include <Hsv.hpp>
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <Console.hpp>
#include <StringUtils.hpp>

#include <iostream>

PaletteDemoScreen::PaletteDemoScreen(octo::IColorProvider const* colors) :
	m_colors(colors)
{
	updateColors();
}

void	PaletteDemoScreen::setColors(octo::IColorProvider const* colors)
{
	m_colors = colors;
	updateColors();
}

void	PaletteDemoScreen::updateColors()
{
	if (m_colors == nullptr)
		return;
	sf::Vector2f		pos(-200, -200);
	sf::RectangleShape	rect(sf::Vector2f(32, 32));

	m_rectangles.clear();
	rect.setOrigin(sf::Vector2f(16, 16));
	for (std::size_t i = 0; i < m_colors->getColorCount(); ++i)
	{
		if (pos.x > 200.f)
		{
			pos.x = -200.f;
			pos.y += 32.f;
		}
		rect.setPosition(pos);
		rect.setFillColor(m_colors->getColor(i));
		rect.setOutlineThickness(1.f);
		rect.setOutlineColor(sf::Color::White);
		m_rectangles.push_back(rect);
		pos.x += 32.f;
	}
}

void	PaletteDemoScreen::start()
{
	octo::Console&	console = octo::Application::getConsole();

	m_view = octo::Application::getGraphicsManager().getDefaultView();
	m_view.setCenter(sf::Vector2f());
	console.addCommand(L"demo.select_palette", [this](std::string const& key)
		{
			octo::ResourceManager&	resources = octo::Application::getResourceManager();
			octo::Console&	console = octo::Application::getConsole();

			try
			{
				setColors(&resources.getPalette(key));
			}
			catch(std::range_error const& e)
			{
				console.print(octo::stringToWide(e.what()), octo::Console::ErrorColor);
			}
		});
	console.addCommand(L"demo.select_wheel", [this](std::string const& key)
		{
			octo::ResourceManager&	resources = octo::Application::getResourceManager();
			octo::Console&	console = octo::Application::getConsole();

			try
			{
				setColors(&resources.getColorWheel(key));
			}
			catch(std::range_error const& e)
			{
				console.print(octo::stringToWide(e.what()), octo::Console::ErrorColor);
			}
		});
	console.print(L" $> demo.select_wheel(\"<wheel_file_name>\")", octo::Console::HelpColor);
	console.print(L" $> demo.select_palette(\"<palette_file_name>\")", octo::Console::HelpColor);
	console.print(L"Usage:", octo::Console::HelpColor);
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
