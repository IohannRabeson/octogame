/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflyTestScreen.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 18:00:29 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/19 20:24:26 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FireflyTestScreen.hpp"
#include "FireflyPositionBehaviors.hpp"
#include "../ResourceDefinitions.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <Console.hpp>
#include <WPrintSFML.hpp>

FireflyTestScreen::FireflyTestScreen() :
	m_swarm(10000),
	m_uniformPopulation(1234u, sf::Color(233, 213, 61),
						1.2f, 2.f, 6.f, 10.f, 32.f, 50.f,
						sf::Time::Zero, sf::Time::Zero),
	m_spawnMode(FireflySwarm::SpawnMode::Normal),
	m_color(233, 213, 61),
	m_speed(1.f),
	m_radius(8.f),
	m_haloRadius(32.f)
{
	octo::Console&	console = octo::Application::getConsole();

	// Setup handles
	m_handles.resize(2);
	m_handles[0] = PointHandle(sf::Color::Red, 8.f, std::bind(&FireflySwarm::setTarget, &m_swarm, std::placeholders::_1));
	m_handles[1] = PointHandle(sf::Color::Green, 8.f, [this](sf::Vector2f const& pos){m_spawn = pos;});
	// Setup swarm
	m_behavior = new FireflySwarm::CirclePositionBehavior(2345, 200.f);
	m_swarm.setPositionBehavior(m_behavior);
	m_swarm.setTexture(octo::Application::getResourceManager().getTexture(FIREFLY01_PNG));

	// Setup console
	console.addCommand(L"sp", [this]()
			{
				return (m_swarm.create(m_spawnMode, m_spawn, m_color, m_radius, m_haloRadius, m_speed));
			});

	console.addCommand(L"spN", [this](std::size_t count)
			{
				for (std::size_t i = 0; i < count; ++i)
					m_swarm.create(m_spawnMode, m_spawn, m_color, m_radius, m_haloRadius, m_speed);
			});

	console.addCommand(L"uspN", [this](std::size_t count)
			{
				for (std::size_t i = 0; i < count; ++i)
					m_swarm.create(m_spawnMode, m_spawn, m_uniformPopulation);
			});

	console.addCommand(L"killAll", m_swarm, &FireflySwarm::killAll);
	console.addCommand(L"kill", m_swarm, &FireflySwarm::kill);
	console.addCommand(L"count", m_swarm, &FireflySwarm::getCount);
	console.addCommand(L"capacity", m_swarm, &FireflySwarm::getCapacity);
	console.addCommand(L"setColor", [this](sf::Color const& color)
			{
				m_color = color;
				m_uniformPopulation.setColor(color);
			});
	console.addCommand(L"getColor", [this](){return (m_color);});
	console.addCommand(L"setSpeed", [this](float value){m_speed = value;});
	console.addCommand(L"getSpeed", [this](){return (m_speed);});
	console.addCommand(L"setRadius", [this](float value){m_radius = value;});
	console.addCommand(L"setHaloRadius", [this](float value){m_haloRadius = value;});
	console.addCommand(L"getHaloRadius", [this](){return (m_haloRadius);});
	console.addCommand(L"setBehaviorRadius", m_behavior, &FireflySwarm::CirclePositionBehavior::setRadius);
	console.addCommand(L"getBehaviorRadius", m_behavior, &FireflySwarm::CirclePositionBehavior::getRadius);
	console.addCommand(L"setSpawnMode",
						[this](std::wstring const& value) -> std::wstring 
						{
							if (value == L"static")
								m_spawnMode = FireflySwarm::SpawnMode::Static;
							else if (value == L"lazy")
								m_spawnMode = FireflySwarm::SpawnMode::Lazy;
							else if (value == L"normal")
								m_spawnMode = FireflySwarm::SpawnMode::Normal;
							else if (value == L"stressed")
								m_spawnMode = FireflySwarm::SpawnMode::Stressed;
							else
								return (L"invalid spawn mode: " + value);
							return (L"spawn mode changed");
						});
	console.addCommand(L"getSpawnMode",
						[this]()
						{
							std::wstring	result = L"invalid spawn mode";

							switch (m_spawnMode)
							{
								case FireflySwarm::SpawnMode::Stressed:
									result = L"stressed";
									break;
								case FireflySwarm::SpawnMode::Normal:
									result = L"normal";
									break;
								case FireflySwarm::SpawnMode::Lazy:
									result = L"lazy";
									break;
								case FireflySwarm::SpawnMode::Static:
									result = L"static";
									break;
								default:
									break;
							}
							return (result);
						});
	console.addCommand(L"setMinSpeed", m_uniformPopulation, &FireflySwarm::UniformPopulation::setMinSpeed);
	console.addCommand(L"setMaxSpeed", m_uniformPopulation, &FireflySwarm::UniformPopulation::setMaxSpeed);
	console.addCommand(L"setMinRadius", m_uniformPopulation, &FireflySwarm::UniformPopulation::setMinRadius);
	console.addCommand(L"setMaxRadius", m_uniformPopulation, &FireflySwarm::UniformPopulation::setMaxRadius);
	console.addCommand(L"setMinHaloRadius", m_uniformPopulation, &FireflySwarm::UniformPopulation::setMinHaloRadius);
	console.addCommand(L"setMaxHaloRadius", m_uniformPopulation, &FireflySwarm::UniformPopulation::setMaxHaloRadius);
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
	for (auto it = m_handles.rbegin(); it != m_handles.rend(); ++it)
		render.draw(*it);
	m_swarm.draw(render);
}

void	FireflyTestScreen::onMoved(sf::Event::MouseMoveEvent const& event)
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();
	sf::Vector2f		pos = graphics.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	for (PointHandle& handle : m_handles)
	{
		handle.setPosition(pos);
	}
}

void	FireflyTestScreen::onPressed(sf::Event::MouseButtonEvent const& event)
{
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();
	sf::Vector2f			pos = graphics.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	switch (event.button)
	{
		case sf::Mouse::Left:
			for (PointHandle& handle : m_handles)
			{
				if (handle.hitTest(pos))
				{
					handle.setHandled(true);
					break;
				}
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
			for (PointHandle& handle : m_handles)
			{
				handle.setHandled(false);
			}
			break;
		default:
			break;
	}
}

void	FireflyTestScreen::onWheel(sf::Event::MouseWheelEvent const&)
{

}
