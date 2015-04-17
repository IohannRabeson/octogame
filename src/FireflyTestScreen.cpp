/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflyTestScreen.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 18:00:29 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/17 18:37:59 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FireflyTestScreen.hpp"
#include "../ResourceDefinitions.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <Console.hpp>
#include <WPrintSFML.hpp>

FireflyTestScreen::FireflyTestScreen() :
	m_swarm(10000)
{
	octo::Console&	console = octo::Application::getConsole();

//	m_swarm.setPositionBehavior(new FireflySwarm::RectanglePositionBehavior(21212, 128.f, 128.f));
	m_swarm.setPositionBehavior(new FireflySwarm::CirclePositionBehavior(21212, 200.f));
//	m_swarm.setPositionBehavior(new FireflySwarm::RingPositionBehavior(21212, 800.f, 600.f));
	m_swarm.setTexture(octo::Application::getResourceManager().getTexture(FIREFLY01_PNG));
	console.addCommand(L"sp", [this](){return m_swarm.create(FireflySwarm::SpawnMode::Lazy, sf::Vector2f(), sf::Color::Red, 8.f, 1.f);});
	console.addCommand(L"spN", [this](std::size_t count)
			{
				for (std::size_t i = 0; i < count; ++i)
					m_swarm.create(FireflySwarm::SpawnMode::Stressed, sf::Vector2f(), sf::Color(233, 213, 61), 8.f, 1.f);
			});
	console.addCommand(L"kill", m_swarm, &FireflySwarm::killAll);
	console.addCommand(L"count", m_swarm, &FireflySwarm::getCount);
	console.addCommand(L"capacity", m_swarm, &FireflySwarm::getCapacity);
}

void	FireflyTestScreen::start()
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();

	m_view = graphics.getDefaultView();
	m_view.setCenter(sf::Vector2f());
	graphics.addMouseListener(this);
}

void	FireflyTestScreen::pause()
{
}

void	FireflyTestScreen::resume()
{
}

void	FireflyTestScreen::stop()
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();

	graphics.removeMouseListener(this);
}

void	FireflyTestScreen::update(sf::Time frameTime)
{
	m_swarm.update(frameTime);
}

void	FireflyTestScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	render.setView(m_view);
	render.draw(m_interestPoint);
	m_swarm.draw(render);
}

void	FireflyTestScreen::onMoved(sf::Event::MouseMoveEvent const& event)
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();
	sf::Vector2f		pos = graphics.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	m_interestPoint.setPosition(pos);
	m_swarm.setTarget(pos);
}

void	FireflyTestScreen::onPressed(sf::Event::MouseButtonEvent const& event)
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();
	sf::Vector2f		pos = graphics.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	switch (event.button)
	{
		case sf::Mouse::Left:
			if (m_interestPoint.hitTest(pos))
			{
				m_interestPoint.setHandled(true);
			}
			break;
		default:
			break;
	}
}

void	FireflyTestScreen::onReleased(sf::Event::MouseButtonEvent const& event)
{
	switch (event.button)
	{
		case sf::Mouse::Left:
			if (m_interestPoint.isHandled())
			{
				m_interestPoint.setHandled(false);
			}
			break;
		default:
			break;
	}
}

void	FireflyTestScreen::onWheel(sf::Event::MouseWheelEvent const&)
{

}
