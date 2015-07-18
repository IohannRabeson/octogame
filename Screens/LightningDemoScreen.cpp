/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LightningDemoScreen.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/23 02:40:01 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/25 17:14:12 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LightningDemoScreen.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <GraphicsManager.hpp>
#include <Console.hpp>

LightningDemoScreen::LightningDemoScreen() :
	m_lightning(200000)
{
}

void	LightningDemoScreen::start()
{
	octo::Camera&			camera = octo::Application::getCamera();
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();
	octo::Console&			console = octo::Application::getConsole();
	sf::Vector2f			offset{0.f, (camera.getSize().y / 2.f) - 64.f};

	graphics.addMouseListener(this);
	m_lightning.addArc(camera.getCenter() - offset, camera.getCenter() + offset, 2.f);
	m_lightning.addArc(camera.getCenter() - offset, camera.getCenter() + offset, 2.f);
	m_lightning.addArc(camera.getCenter() - offset, camera.getCenter() + offset, 2.f);
	m_lightning.addArc(camera.getCenter() - offset, camera.getCenter() + offset, 2.f);
	m_lightning.addArc(camera.getCenter() - offset, camera.getCenter() + offset, 2.f);
	m_lightning.addArc(camera.getCenter() - offset, camera.getCenter() + offset, 2.f);
	m_p0.setMoveCallback([this](sf::Vector2f const&)
						 {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
							{
								m_lightning.setArc(i, m_p0.getPosition(), m_p1.getPosition());
							}
						 });
	m_p1.setMoveCallback([this](sf::Vector2f const&)
						 {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
							{
								m_lightning.setArc(i, m_p0.getPosition(), m_p1.getPosition());
							}
						 });
	m_p0.setPosition(camera.getCenter() - offset, true);
	m_p1.setPosition(camera.getCenter() + offset, true);
	console.addCommand(L"demo.setBranchProbability",
					   [this](float proba)
					   {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
					   			m_lightning.getArc(i).setBranchProbability(proba);
					   });
	console.addCommand(L"demo.setColor",
					   [this](sf::Color const& color)
					   {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
					   			m_lightning.getArc(i).setColor(color);
					   });
	console.addCommand(L"demo.setThickness",
					   [this](float thickness)
					   {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
					   			m_lightning.getArc(i).setThickness(thickness);
					   });
	console.addCommand(L"demo.setMiddleOffsetFactor",
					   [this](float factor)
					   {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
					   			m_lightning.getArc(i).setMiddleOffsetFactor(factor);
					   });
	console.addCommand(L"demo.setFractalLevel",
					   [this](std::size_t level)
					   {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
					   			m_lightning.getArc(i).setFractalLevel(level);
					   });
	console.addCommand(L"demo.setBranchMaxAngle",
					   [this](float maxAngle)
					   {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
					   			m_lightning.getArc(i).setBranchMaxAngle(maxAngle);
					   });
	console.addCommand(L"demo.setBranchMaxLenght",
					   [this](float lenght)
					   {
						 	for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
					   			m_lightning.getArc(i).setBranchMaxLenght(lenght);
					   });
}

void	LightningDemoScreen::pause()
{
}

void	LightningDemoScreen::resume()
{
}

void	LightningDemoScreen::stop()
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();

	graphics.removeMouseListener(this);
}

void	LightningDemoScreen::update(sf::Time frameTime)
{
	m_lightning.update(frameTime);
}

void	LightningDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	m_p0.draw(render);
	m_p1.draw(render);
	m_lightning.draw(render);
}

bool	LightningDemoScreen::onPressed(sf::Event::KeyEvent const& event)
{
	(void)event;
	return (false);
}

bool	LightningDemoScreen::onReleased(sf::Event::KeyEvent const& event)
{
	(void)event;
	return (false);
}

void	LightningDemoScreen::onMoved(sf::Event::MouseMoveEvent const& event)
{
	octo::Camera&	camera = octo::Application::getCamera();
	sf::Vector2f	pos = camera.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	m_p0.setPosition(pos);
	m_p1.setPosition(pos);
}

void	LightningDemoScreen::onPressed(sf::Event::MouseButtonEvent const& event)
{
	octo::Camera&	camera = octo::Application::getCamera();
	sf::Vector2f	pos = camera.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	switch (event.button)
	{
		case sf::Mouse::Left:
			if (m_p0.hitTest(pos))
			{
				m_p0.setHandled(true);
			}
			else if (m_p1.hitTest(pos))
			{
				m_p1.setHandled(true);
			}
			break;
		default:
			break;
	}
}

void	LightningDemoScreen::onReleased(sf::Event::MouseButtonEvent const& event)
{
	octo::Camera&	camera = octo::Application::getCamera();
	sf::Vector2f	pos = camera.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	switch (event.button)
	{
		case sf::Mouse::Left:
			if (m_p0.hitTest(pos))
			{
				m_p0.setHandled(false);
			}
			else if (m_p1.hitTest(pos))
			{
				m_p1.setHandled(false);
			}
			break;
		default:
			break;
	}
}
