/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpriteSheetDemoScreen.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/23 01:27:24 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/23 02:33:42 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SpriteSheetDemoScreen.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <Console.hpp>
#include <Camera.hpp>

#include <iostream>

SpriteSheetDemoScreen::SpriteSheetDemoScreen() :
	m_spriteSheet(nullptr)
{
	octo::Application::getGraphicsManager().addKeyboardListener(this);
}

SpriteSheetDemoScreen::~SpriteSheetDemoScreen()
{
	octo::Application::getGraphicsManager().removeKeyboardListener(this);
}

void	SpriteSheetDemoScreen::start()
{
	octo::Application::getConsole().addCommand(L"demo.set_sheet", [this](std::string const& fileName)
			{
				octo::ResourceManager& resourceManager = octo::Application::getResourceManager();

				setSpriteSheet(&resourceManager.getSpriteSheet(fileName));
			});
}

void	SpriteSheetDemoScreen::pause()
{

}

void	SpriteSheetDemoScreen::resume()
{

}

void	SpriteSheetDemoScreen::stop()
{
}

void	SpriteSheetDemoScreen::update(sf::Time)
{
}

void	SpriteSheetDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	render.draw(m_sprite);
}

bool	SpriteSheetDemoScreen::onPressed(sf::Event::KeyEvent const& event)
{
	if (m_spriteSheet)
	{
		switch (event.code)
		{
			case sf::Keyboard::Left:
				if (m_subTextureId == 0)
					m_subTextureId = m_spriteSheet->getSubRectCount() - 1;
				else
					--m_subTextureId;
				m_sprite.setTextureRect(m_spriteSheet->getSubRect(m_subTextureId));
				break;
			case sf::Keyboard::Right:
				++m_subTextureId;
				if (m_subTextureId >= m_spriteSheet->getSubRectCount())
					m_subTextureId = 0;
				m_sprite.setTextureRect(m_spriteSheet->getSubRect(m_subTextureId));
				break;
			default:
				break;
		}
	}
	return (true);
}

void	SpriteSheetDemoScreen::setSpriteSheet(octo::SpriteSheet const* spriteSheet)
{
	octo::Camera&	camera = octo::Application::getCamera();
	sf::FloatRect	spriteRect;

	m_spriteSheet = spriteSheet;
	m_subTextureId = 0;
	m_sprite.setTexture(m_spriteSheet->getTexture());
	m_sprite.setTextureRect(m_spriteSheet->getSubRect(m_subTextureId));
	spriteRect = m_sprite.getGlobalBounds();
	camera.setCenter(spriteRect.left + spriteRect.width / 2,
					 spriteRect.top + spriteRect.height / 2);
}
